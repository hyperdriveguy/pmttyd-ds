#!/usr/bin/env python3
"""Inventory declared game objects in a supplied public decompilation snapshot.

Parses configure.py without executing it. Matching labels are upstream build
declarations, not independently verified decompilation/behavior coverage.
"""
import argparse
import ast
from collections import Counter
import hashlib
import json
from pathlib import Path


def audit(root):
    root = Path(root)
    config = (root / 'configure.py').read_bytes()
    tree = ast.parse(config)
    libraries = None
    for statement in tree.body:
        if isinstance(statement, ast.Assign) and any(
                isinstance(t, ast.Attribute) and isinstance(t.value, ast.Name)
                and t.value.id == 'config' and t.attr == 'libs'
                for t in statement.targets):
            libraries = statement.value
    if not isinstance(libraries, ast.List):
        raise ValueError('Unsupported config.libs syntax')
    records = []
    for library in libraries.elts:
        if isinstance(library, ast.Dict):
            fields = {ast.literal_eval(k): v for k, v in zip(library.keys, library.values)}
            category = fields.get('progress_category')
            if not isinstance(category, ast.Constant) or category.value != 'game':
                continue
            name = ast.literal_eval(fields['lib'])
            source_dir = ast.literal_eval(fields['src_dir']) if 'src_dir' in fields else 'src'
            objects = fields['objects']
        elif isinstance(library, ast.Call) and isinstance(library.func, ast.Name) and library.func.id == 'Rel':
            name = ast.literal_eval(library.args[0])
            source_dir = f'rels/{name}/src'
            objects = library.args[1]
        else:
            continue  # SDK/library helpers are outside this game-object census.
        if not isinstance(objects, ast.List):
            raise ValueError('Unsupported objects syntax')
        for obj in objects.elts:
            if not isinstance(obj, ast.Call) or not isinstance(obj.func, ast.Name) or obj.func.id != 'Object':
                raise ValueError('Unsupported object declaration')
            state, filename = obj.args[:2]
            path = ast.literal_eval(filename)
            declaration = state.id if isinstance(state, ast.Name) else ast.unparse(state)
            relative = Path(source_dir) / path
            if relative.is_absolute() or '..' in relative.parts:
                raise ValueError('Source path escapes snapshot')
            source = root / relative
            data = source.read_bytes() if source.is_file() else None
            records.append(dict(library=name, path=relative.as_posix(),
                                declaration=declaration, source_present=data is not None,
                                source_size=len(data) if data is not None else None,
                                sha256=hashlib.sha256(data).hexdigest() if data is not None else None,
                                config_line=obj.lineno))
    return dict(schema=1, config_sha256=hashlib.sha256(config).hexdigest(),
                limitation='Declaration/source existence only; no function matching or semantic completeness claim',
                declarations=dict(Counter(r['declaration'] for r in records)),
                present=sum(r['source_present'] for r in records),
                absent=sum(not r['source_present'] for r in records), objects=records)


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--snapshot', required=True)
    parser.add_argument('--output', required=True)
    args = parser.parse_args()
    output = Path(args.output).resolve()
    snapshot = Path(args.snapshot).resolve()
    if output == snapshot or snapshot in output.parents:
        raise ValueError('Write the report outside the reference snapshot')
    result = audit(snapshot)
    output.parent.mkdir(parents=True, exist_ok=True)
    output.write_text(json.dumps(result, indent=2) + '\n')
    print(json.dumps({k: v for k, v in result.items() if k != 'objects'}, indent=2))


if __name__ == '__main__':
    main()
