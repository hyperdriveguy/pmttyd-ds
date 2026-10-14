import json,struct,unittest
from pathlib import Path

@unittest.skipUnless(Path('romfs/gor_02.mapflags').is_file(),'local original map flags unavailable')
class FieldMapFlagsAssets(unittest.TestCase):
 def test_original_hierarchy_collision_and_triangle_ownership(self):
  raw=Path('romfs/gor_02.mapflags').read_bytes();magic,vertices,count=struct.unpack_from('<8sII',raw)
  self.assertEqual(magic,b'PMFLG001');self.assertEqual(vertices*36,Path('romfs/gor_02.vertices').stat().st_size)
  summary=json.loads(Path('local-assets/gor_02/map-summary.json').read_text());original={n['source_node']:n for n in summary['collision_nodes']}
  nodes=[]
  for n in range(count):
   parent,source,hit,name=struct.unpack_from('<iII64s',raw,16+n*76);name=name.split(b'\0')[0].decode()
   self.assertTrue(parent==-1 or 0<=parent<n);self.assertIn(hit,(0,1));self.assertEqual(bool(hit),source in original)
   if hit:self.assertEqual(name,original[source]['name'])
   nodes.append((parent,source,hit,name))
  owners=struct.unpack_from('<'+'H'*vertices,raw,16+count*76);self.assertEqual(len(raw),16+76*count+2*vertices)
  for n in range(0,vertices,3):self.assertTrue(owners[n]<count and owners[n]==owners[n+1]==owners[n+2])
  # Expected house05 subtree from original parent identities, not a material match.
  hitroot=next(n['source_node'] for n in original.values() if n['name']=='A_ie_05C');hits={hitroot}
  while True:
   expanded=hits|{s for s,n in original.items() if n['parent'] in hits}
   if expanded==hits:break
   hits=expanded
  root=next(i for i,n in enumerate(nodes) if n[3]=='A_ie_05C');derived=set()
  for i,node in enumerate(nodes):
   ancestor=i
   while ancestor>=0 and ancestor!=root:ancestor=nodes[ancestor][0]
   if ancestor==root and node[2]:derived.add(node[1])
  self.assertEqual(derived,hits)
  self.assertTrue(any(nodes[o][3]=='S_ie_05_naka' for o in owners) or any(n[3]=='S_ie_05_naka' for n in nodes))
