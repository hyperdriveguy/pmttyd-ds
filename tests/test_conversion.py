from pathlib import Path
import sys
sys.path.insert(0,str(Path(__file__).resolve().parents[1]/'tools'))
import unittest
import tempfile
import struct
import numpy as np
from audio_convert import decode,nibbles_to_samples
from character_bake import matrix
from map_convert import topology,wall_segments,batch_flags
from map_animation import interpolate,sample,read_animations
class ConversionTests(unittest.TestCase):
    def test_dsp_predictor_zero(self):
        self.assertEqual(list(decode(bytes([0,0x1f])+bytes(6),14,[0]*16))[:3],[1,-1,0])
    def test_dsp_history(self):
        coefficients=[2048,0]+[0]*14
        self.assertEqual(list(decode(bytes(8),14,coefficients,10,0)),[10]*14)
    def test_dsp_clamp_and_partial(self):
        self.assertEqual(list(decode(bytes([0x0f,0x77])+bytes(6),2,[0]*16)),[32767,32767])
        with self.assertRaises(ValueError):decode(bytes(7),14,[0]*16)
        with self.assertRaises(ValueError):decode(bytes([0x80])+bytes(7),14,[0]*16)
    def test_dsp_addressing(self):
        self.assertEqual(nibbles_to_samples(2),0);self.assertEqual(nibbles_to_samples(18),14)
    def test_character_transform(self):
        node=np.zeros(24);node[:3]=[1,2,3];node[3:6]=[2,2,2]
        self.assertTrue(np.allclose(matrix(node,0)@np.array([1,1,1,1]),[3,4,5,1]))
    def test_triangle_strip(self):
        self.assertEqual(list(topology(0x98,[0,1,2,3])),[[0,1,2],[2,1,3]])
    def test_stacked_wall_segments(self):
        low=[[0,0,0],[2,0,0],[0,1,0]]
        high=[[0,3,0],[2,3,0],[0,4,0]]
        segments=wall_segments([low,high,low])
        self.assertEqual(len(segments),2)
        self.assertEqual([s[4:] for s in segments],[(0.,1.),(3.,4.)])
    def test_batch_metadata(self):
        for layer in [0,2,255]:
            for group in [0,26,1024,65535]:
                for cull in range(4):
                    flags=batch_flags(layer,group,cull)
                    self.assertEqual(flags&255,layer)
                    self.assertEqual((flags>>8)&65535,group)
                    self.assertEqual(flags>>24,cull)
        for args in [(-1,0,0),(256,0,0),(0,-1,0),(0,65536,0),(0,0,4)]:
            with self.assertRaises(ValueError):batch_flags(*args)
    def test_map_animation_reader(self):
        raw=bytearray(960)
        def put(offset,fmt,*values):struct.pack_into('>'+fmt,raw,offset,*values)
        put(4,'I',64);put(12,'I',1);put(96,'II',128,0)
        raw[104:120]=b'animation_table\0'
        put(160,'II',1,160);put(192,'I',848);put(200,'f',10);put(204,'I',224)
        put(256,'II',1,288);put(320,'I',868)
        put(324,'9f',0,0,0,0,0,0,1,1,1);put(408,'I',1)
        for n,value in enumerate([2,0,0,0,0,0,1,1,1]):put(416+n*20,'f',value)
        raw[880:885]=b'idle\0';raw[900:905]=b'door\0'
        with tempfile.TemporaryDirectory() as directory:
            path=Path(directory)/'map.d';path.write_bytes(raw)
            animation=read_animations(path)[0]
            self.assertEqual(animation['name'],'idle');self.assertEqual(animation['tracks'][0]['node'],'door')
            self.assertTrue(np.allclose(sample(animation['tracks'][0],5)@np.array([0,0,0,1]),[2,0,0,1]))
            for offset,fmt,value in [(200,'f',float('nan')),(348,'f',0),(412,'f',float('inf')),(432,'I',2),(408,'I',10001)]:
                broken=bytearray(raw);struct.pack_into('>'+fmt,broken,offset,value);path.write_bytes(broken)
                with self.assertRaises(ValueError):read_animations(path)
            path.write_bytes(raw[:300])
            with self.assertRaises(ValueError):read_animations(path)
    def test_map_animation_interpolation(self):
        self.assertEqual(interpolate([2,0,0,True],[8,0,0,False],.5,10),2)
        self.assertEqual(interpolate([2,0,0,False],[8,0,0,False],.5,10),5)
        self.assertEqual(interpolate([0,0,1,False],[10,1,0,False],.5,10),5)
        def components(x):return [[v,0,0,False] for v in [x,0,0,0,0,0,2,3,4]]
        track={'offsets':[1,0,0,0,0,0,2,3,4], 'frames':[{'time':0,'components':components(1)},{'time':10,'components':components(11)}]}
        self.assertTrue(np.allclose(sample(track,-10),np.eye(4)))
        self.assertTrue(np.allclose(sample(track,5)@np.array([0,0,0,1]),[5,0,0,1]))
        self.assertTrue(np.allclose(sample(track,100)@np.array([0,0,0,1]),[10,0,0,1]))
        with self.assertRaises(ValueError):sample(track,float('nan'))
    def test_triangle_fan(self):
        self.assertEqual(list(topology(0xa0,[0,1,2,3])),[[0,1,2],[0,2,3]])
if __name__=='__main__':unittest.main()
