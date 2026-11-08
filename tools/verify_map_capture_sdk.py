#!/usr/bin/env python3
"""Verify original complete GX map capture SDK bodies and initialized data."""
import argparse,hashlib,json
from pathlib import Path
from field_map_npc_assets import Dol
from iso_assets import DEFAULT_ISO
ROOT=Path(__file__).resolve().parents[1]
def verify(iso=DEFAULT_ISO):
 evidence=json.loads((ROOT/'tools/map_capture_sdk_evidence.json').read_text());dol=Dol(iso);count=0
 try:
  for body in evidence['bodies']:
   raw=dol.read(int(body['address'],16),body['size'])
   if hashlib.sha256(raw).hexdigest()!=body['sha256']:raise ValueError('Changed source body: '+body['name'])
   count+=len(raw)//4
  for span in evidence['initialized_spans']:
   raw=dol.read(int(span['address'],16),span['bytes'])
   if raw.hex()!=span['hex'] or hashlib.sha256(raw).hexdigest()!=span['sha256']:raise ValueError('Changed initialized capture data')
 finally:dol.close()
 return len(evidence['bodies']),count
if __name__=='__main__':
 p=argparse.ArgumentParser(description=__doc__);p.add_argument('--iso',default=DEFAULT_ISO);args=p.parse_args();bodies,count=verify(args.iso);print('Verified',bodies,'complete GX capture SDK bodies;',count,'original instructions')
