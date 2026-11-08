"""Append-only editable animation clips and original pose-directory recipes."""
import copy
from pathlib import Path
from battle_pose_directory import write_directory
POSE_SUFFIX = '.pose-directory.bin'

def auxiliary_sources(entry):
    result = []
    if 'append_sources' in entry:
        paths = entry['append_sources']
        if entry['kind'] != 'animation' or not isinstance(paths, list) or not paths:
            raise ValueError('append_sources requires a nonempty animation source list')
        if any(not isinstance(path, str) or not path for path in paths):
            raise ValueError('Animation append sources must be relative source paths')
        if len(set(paths)) != len(paths) or entry['source'] in paths:
            raise ValueError('Animation append sources must be distinct from each other and primary')
        result.extend(paths)
    if entry['kind'] == 'pose-directory':
        texture = entry.get('texture_source')
        if not isinstance(texture, str) or not texture:
            raise ValueError('Pose-directory recipe requires original texture_source')
        result.append(texture)
    return result

def append_animation_clips(primary, additions):
    result = copy.deepcopy(primary)
    names = {clip['name'] for clip in result['clips']}
    for addition in additions:
        if addition.get('schema') != primary.get('schema') or addition.get('format') != primary.get('format'):
            raise ValueError('Animation append format must match the current primary source')
        for clip in addition['clips']:
            if clip['name'] in names:
                raise ValueError('Animation append collides with an existing clip: '+clip['name'])
            names.add(clip['name']);result['clips'].append(copy.deepcopy(clip))
    return result

def pose_directory_outputs(output):
    if not output.endswith(POSE_SUFFIX):
        raise ValueError('Pose-directory output requires .pose-directory.bin suffix')
    prefix = output[:-len(POSE_SUFFIX)]
    return [output, prefix+'.draw-model.bin', prefix+'.draw-textures.bin']

def compile_pose_directory(model, target, texture):
    # These are original MODEL/TPL source assets. Only the directory/discrete
    # metadata is encoded; companions remain exact source-byte copies.
    target = Path(target)
    target.parent.mkdir(parents=True, exist_ok=True)
    write_directory(model, target, texture)
