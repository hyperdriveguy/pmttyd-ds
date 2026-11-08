// Complete US revision0 callable bodies, compiled fixed continuation graph.
// Verify all original instruction words with tools/verify_field_map_source.py.
// mapInit 8002af18/44 SHA256 b3af1e5fe3daca20b8be3327af0489a2fc1215b6077364e24518bae80eacf1a3
// mapGetMapObj 8001f878/a8 SHA256 08479b2fd692ed36be2616e3e7fc5a636843f2b8b50b8a18d63e3e55b95a5c08
// mapEntry 8002a46c/9c SHA256 6710a39ca6579f63f7bffc9b19399ffeaf38eb00b51106819dcc8c791cc6045a
// mapEntrySub 8002a508/278 SHA256 5c189fec9beaeec8588e1b57259c98b51001209076f4d5f994520c23b5bd9967
// mapGetJoints 8002b120/4c SHA256 793319dc9c5b675b7b83ec342f19f4ec9d1ce828f86ba6a966427a3c133c1f73
// mapGetJointsSub 8002b16c/2c4 SHA256 54af1c20567bbfdf736fa2c05fedd46a748cb0513ab793d0068a693dd5344cd9
// _mapEnt 8002a780/588 SHA256 3bb8d600e61cc48de1452ddb60f6e63e4ba496cbd2dc5720ce19cbf126f06678
// mapReCalcMatrix 80028130/420 SHA256 212353de648b9515c7c9618b7faa4210044a37855eb11ab28fa2d389388d75f1
case 0x8002af18:{const auto target=(r[1]+uint32_t(-16));if(!store(target,r[1]))return {};r[1]=target;c.pc=0x8002af1c;break;} // 9421fff0 stwu 1, -16(1)
case 0x8002af1c:{r[0]=c.lr;c.pc=0x8002af20;break;} // 7c0802a6 mflr 0
case 0x8002af20:{r[3]=uint32_t(0);c.pc=0x8002af24;break;} // 38600000 li 3, 0
case 0x8002af24:{r[4]=uint32_t(1512);c.pc=0x8002af28;break;} // 388005e8 li 4, 1512
case 0x8002af28:{if(!store((r[1]+uint32_t(20)),r[0]))return {};c.pc=0x8002af2c;break;} // 90010014 stw 0, 20(1)
case 0x8002af2c:{if(!external(0x800300f0,0x8002af30))return {};break;} // 480051c5 bl 0x800300f0
case 0x8002af30:{if(!store((r[13]+uint32_t(5952)),r[3]))return {};c.pc=0x8002af34;break;} // 906d1740 stw 3, 5952(13)
case 0x8002af34:{r[4]=uint32_t(0);c.pc=0x8002af38;break;} // 38800000 li 4, 0
case 0x8002af38:{r[5]=uint32_t(1512);c.pc=0x8002af3c;break;} // 38a005e8 li 5, 1512
case 0x8002af3c:{if(!external(0x800050b4,0x8002af40))return {};break;} // 4bfda179 bl 0x800050b4
case 0x8002af40:{r[0]=uint32_t(0);c.pc=0x8002af44;break;} // 38000000 li 0, 0
case 0x8002af44:{if(!store((r[13]+uint32_t(5956)),r[0]))return {};c.pc=0x8002af48;break;} // 900d1744 stw 0, 5956(13)
case 0x8002af48:{if(!store((r[13]+uint32_t(5960)),r[0]))return {};c.pc=0x8002af4c;break;} // 900d1748 stw 0, 5960(13)
case 0x8002af4c:{if(!word((r[1]+uint32_t(20)),r[0]))return {};c.pc=0x8002af50;break;} // 80010014 lwz 0, 20(1)
case 0x8002af50:{c.lr=r[0];c.pc=0x8002af54;break;} // 7c0803a6 mtlr 0
case 0x8002af54:{r[1]=r[1]+uint32_t(16);c.pc=0x8002af58;break;} // 38210010 addi 1, 1, 16
case 0x8002af58:{c.pc=c.lr;break;} // 4e800020 blr 
case 0x8001f878:{const auto target=(r[1]+uint32_t(-32));if(!store(target,r[1]))return {};r[1]=target;c.pc=0x8001f87c;break;} // 9421ffe0 stwu 1, -32(1)
case 0x8001f87c:{r[0]=c.lr;c.pc=0x8001f880;break;} // 7c0802a6 mflr 0
case 0x8001f880:{if(!store((r[1]+uint32_t(36)),r[0]))return {};c.pc=0x8001f884;break;} // 90010024 stw 0, 36(1)
case 0x8001f884:{while(c.step<6){const auto index=26+c.step;if(!store((r[1]+uint32_t(8))+4*c.step,r[index]))return {};++c.step;}c.step=0;c.pc=0x8001f888;break;} // bf410008 stmw 26, 8(1)
case 0x8001f888:{r[26]=r[3];compare(c,0,signedWord(r[26]),0);c.pc=0x8001f88c;break;} // 7c7a1b79 mr. 26, 3
case 0x8001f88c:{if(!word((r[13]+uint32_t(5956)),r[0]))return {};c.pc=0x8001f890;break;} // 800d1744 lwz 0, 5956(13)
case 0x8001f890:{if(!word((r[13]+uint32_t(5952)),r[3]))return {};c.pc=0x8001f894;break;} // 806d1740 lwz 3, 5952(13)
case 0x8001f894:{r[0]=r[0]*uint32_t(756);c.pc=0x8001f898;break;} // 1c0002f4 mulli 0, 0, 756
case 0x8001f898:{r[31]=r[3]+r[0];c.pc=0x8001f89c;break;} // 7fe30214 add 31, 3, 0
case 0x8001f89c:{c.pc=!c.condition[2]?0x8001f8a8:0x8001f8a0;break;} // 4082000c bf 2, 0x8001f8a8
case 0x8001f8a0:{r[3]=uint32_t(0);c.pc=0x8001f8a4;break;} // 38600000 li 3, 0
case 0x8001f8a4:{c.pc=0x8001f90c;break;} // 48000068 b 0x8001f90c
case 0x8001f8a8:{r[30]=r[31];c.pc=0x8001f8ac;break;} // 7ffefb78 mr 30, 31
case 0x8001f8ac:{r[28]=uint32_t(0);c.pc=0x8001f8b0;break;} // 3b800000 li 28, 0
case 0x8001f8b0:{c.pc=0x8001f8fc;break;} // 4800004c b 0x8001f8fc
case 0x8001f8b4:{if(!word((r[30]+uint32_t(340)),r[27]))return {};c.pc=0x8001f8b8;break;} // 837e0154 lwz 27, 340(30)
case 0x8001f8b8:{r[29]=uint32_t(0);c.pc=0x8001f8bc;break;} // 3ba00000 li 29, 0
case 0x8001f8bc:{c.pc=0x8001f8e8;break;} // 4800002c b 0x8001f8e8
case 0x8001f8c0:{if(!word((r[27]+uint32_t(8)),r[3]))return {};c.pc=0x8001f8c4;break;} // 807b0008 lwz 3, 8(27)
case 0x8001f8c4:{r[4]=r[26];c.pc=0x8001f8c8;break;} // 7f44d378 mr 4, 26
case 0x8001f8c8:{if(!word((r[3]+uint32_t(0)),r[3]))return {};c.pc=0x8001f8cc;break;} // 80630000 lwz 3, 0(3)
case 0x8001f8cc:{if(!external(0x8026d464,0x8001f8d0))return {};break;} // 4824db99 bl 0x8026d464
case 0x8001f8d0:{compare(c,0,signedWord(r[3]),int64_t(0));c.pc=0x8001f8d4;break;} // 2c030000 cmpwi 3, 0
case 0x8001f8d4:{c.pc=!c.condition[2]?0x8001f8e0:0x8001f8d8;break;} // 4082000c bf 2, 0x8001f8e0
case 0x8001f8d8:{r[3]=r[27];c.pc=0x8001f8dc;break;} // 7f63db78 mr 3, 27
case 0x8001f8dc:{c.pc=0x8001f90c;break;} // 48000030 b 0x8001f90c
case 0x8001f8e0:{r[29]=r[29]+uint32_t(1);c.pc=0x8001f8e4;break;} // 3bbd0001 addi 29, 29, 1
case 0x8001f8e4:{r[27]=r[27]+uint32_t(308);c.pc=0x8001f8e8;break;} // 3b7b0134 addi 27, 27, 308
case 0x8001f8e8:{if(!word((r[30]+uint32_t(336)),r[0]))return {};c.pc=0x8001f8ec;break;} // 801e0150 lwz 0, 336(30)
case 0x8001f8ec:{compare(c,0,signedWord(r[29]),signedWord(r[0]));c.pc=0x8001f8f0;break;} // 7c1d0000 cmpw 29, 0
case 0x8001f8f0:{c.pc=c.condition[0]?0x8001f8c0:0x8001f8f4;break;} // 4180ffd0 bt 0, 0x8001f8c0
case 0x8001f8f4:{r[30]=r[30]+uint32_t(376);c.pc=0x8001f8f8;break;} // 3bde0178 addi 30, 30, 376
case 0x8001f8f8:{r[28]=r[28]+uint32_t(1);c.pc=0x8001f8fc;break;} // 3b9c0001 addi 28, 28, 1
case 0x8001f8fc:{if(!word((r[31]+uint32_t(0)),r[0]))return {};c.pc=0x8001f900;break;} // 801f0000 lwz 0, 0(31)
case 0x8001f900:{compare(c,0,signedWord(r[28]),signedWord(r[0]));c.pc=0x8001f904;break;} // 7c1c0000 cmpw 28, 0
case 0x8001f904:{c.pc=c.condition[0]?0x8001f8b4:0x8001f908;break;} // 4180ffb0 bt 0, 0x8001f8b4
case 0x8001f908:{r[3]=uint32_t(0);c.pc=0x8001f90c;break;} // 38600000 li 3, 0
case 0x8001f90c:{while(c.step<6){const auto index=26+c.step;if(!word((r[1]+uint32_t(8))+4*c.step,r[index]))return {};++c.step;}c.step=0;c.pc=0x8001f910;break;} // bb410008 lmw 26, 8(1)
case 0x8001f910:{if(!word((r[1]+uint32_t(36)),r[0]))return {};c.pc=0x8001f914;break;} // 80010024 lwz 0, 36(1)
case 0x8001f914:{c.lr=r[0];c.pc=0x8001f918;break;} // 7c0803a6 mtlr 0
case 0x8001f918:{r[1]=r[1]+uint32_t(32);c.pc=0x8001f91c;break;} // 38210020 addi 1, 1, 32
case 0x8001f91c:{c.pc=c.lr;break;} // 4e800020 blr 
case 0x8002a46c:{const auto target=(r[1]+uint32_t(-32));if(!store(target,r[1]))return {};r[1]=target;c.pc=0x8002a470;break;} // 9421ffe0 stwu 1, -32(1)
case 0x8002a470:{r[0]=c.lr;c.pc=0x8002a474;break;} // 7c0802a6 mflr 0
case 0x8002a474:{if(!store((r[1]+uint32_t(36)),r[0]))return {};c.pc=0x8002a478;break;} // 90010024 stw 0, 36(1)
case 0x8002a478:{while(c.step<6){const auto index=26+c.step;if(!store((r[1]+uint32_t(8))+4*c.step,r[index]))return {};++c.step;}c.step=0;c.pc=0x8002a47c;break;} // bf410008 stmw 26, 8(1)
case 0x8002a47c:{r[26]=r[3];c.pc=0x8002a480;break;} // 7c7a1b78 mr 26, 3
case 0x8002a480:{r[27]=r[4];c.pc=0x8002a484;break;} // 7c9b2378 mr 27, 4
case 0x8002a484:{r[28]=r[5];c.pc=0x8002a488;break;} // 7cbc2b78 mr 28, 5
case 0x8002a488:{if(!word((r[13]+uint32_t(5956)),r[0]))return {};c.pc=0x8002a48c;break;} // 800d1744 lwz 0, 5956(13)
case 0x8002a48c:{if(!word((r[13]+uint32_t(5952)),r[6]))return {};c.pc=0x8002a490;break;} // 80cd1740 lwz 6, 5952(13)
case 0x8002a490:{r[0]=r[0]*uint32_t(756);c.pc=0x8002a494;break;} // 1c0002f4 mulli 0, 0, 756
case 0x8002a494:{r[31]=r[6]+r[0];c.pc=0x8002a498;break;} // 7fe60214 add 31, 6, 0
case 0x8002a498:{c.lr=0x8002a49c;c.pc=0x8002b120;break;} // 48000c89 bl 0x8002b120
case 0x8002a49c:{r[30]=r[28]*uint32_t(376);c.pc=0x8002a4a0;break;} // 1fdc0178 mulli 30, 28, 376
case 0x8002a4a0:{r[4]=r[31]+r[30];c.pc=0x8002a4a4;break;} // 7c9ff214 add 4, 31, 30
case 0x8002a4a4:{if(!store((r[4]+uint32_t(336)),r[3]))return {};c.pc=0x8002a4a8;break;} // 90640150 stw 3, 336(4)
case 0x8002a4a8:{if(!word((r[4]+uint32_t(336)),r[0]))return {};c.pc=0x8002a4ac;break;} // 80040150 lwz 0, 336(4)
case 0x8002a4ac:{if(!word((r[13]+uint32_t(6376)),r[3]))return {};c.pc=0x8002a4b0;break;} // 806d18e8 lwz 3, 6376(13)
case 0x8002a4b0:{r[4]=r[0]*uint32_t(308);c.pc=0x8002a4b4;break;} // 1c800134 mulli 4, 0, 308
case 0x8002a4b4:{r[0]=r[4]+uint32_t(31);c.pc=0x8002a4b8;break;} // 3804001f addi 0, 4, 31
case 0x8002a4b8:{r[29]=rotate(r[0],0)&0xffffffe0u;c.pc=0x8002a4bc;break;} // 541d0034 rlwinm 29, 0, 0, 0, 26
case 0x8002a4bc:{r[4]=r[29];c.pc=0x8002a4c0;break;} // 7fa4eb78 mr 4, 29
case 0x8002a4c0:{if(!external(0x8002ff58,0x8002a4c4))return {};break;} // 48005a99 bl 0x8002ff58
case 0x8002a4c4:{r[6]=r[31]+r[30];c.pc=0x8002a4c8;break;} // 7cdff214 add 6, 31, 30
case 0x8002a4c8:{r[5]=r[29];c.pc=0x8002a4cc;break;} // 7fa5eb78 mr 5, 29
case 0x8002a4cc:{if(!store((r[6]+uint32_t(340)),r[3]))return {};c.pc=0x8002a4d0;break;} // 90660154 stw 3, 340(6)
case 0x8002a4d0:{r[4]=uint32_t(0);c.pc=0x8002a4d4;break;} // 38800000 li 4, 0
case 0x8002a4d4:{if(!word((r[6]+uint32_t(340)),r[3]))return {};c.pc=0x8002a4d8;break;} // 80660154 lwz 3, 340(6)
case 0x8002a4d8:{if(!external(0x800050b4,0x8002a4dc))return {};break;} // 4bfdabdd bl 0x800050b4
case 0x8002a4dc:{r[3]=r[26];c.pc=0x8002a4e0;break;} // 7f43d378 mr 3, 26
case 0x8002a4e0:{r[5]=r[27];c.pc=0x8002a4e4;break;} // 7f65db78 mr 5, 27
case 0x8002a4e4:{r[7]=r[28];c.pc=0x8002a4e8;break;} // 7f87e378 mr 7, 28
case 0x8002a4e8:{r[4]=uint32_t(0);c.pc=0x8002a4ec;break;} // 38800000 li 4, 0
case 0x8002a4ec:{r[6]=uint32_t(1);c.pc=0x8002a4f0;break;} // 38c00001 li 6, 1
case 0x8002a4f0:{c.lr=0x8002a4f4;c.pc=0x8002a508;break;} // 48000019 bl 0x8002a508
case 0x8002a4f4:{while(c.step<6){const auto index=26+c.step;if(!word((r[1]+uint32_t(8))+4*c.step,r[index]))return {};++c.step;}c.step=0;c.pc=0x8002a4f8;break;} // bb410008 lmw 26, 8(1)
case 0x8002a4f8:{if(!word((r[1]+uint32_t(36)),r[0]))return {};c.pc=0x8002a4fc;break;} // 80010024 lwz 0, 36(1)
case 0x8002a4fc:{c.lr=r[0];c.pc=0x8002a500;break;} // 7c0803a6 mtlr 0
case 0x8002a500:{r[1]=r[1]+uint32_t(32);c.pc=0x8002a504;break;} // 38210020 addi 1, 1, 32
case 0x8002a504:{c.pc=c.lr;break;} // 4e800020 blr 
case 0x8002a508:{const auto target=(r[1]+uint32_t(-64));if(!store(target,r[1]))return {};r[1]=target;c.pc=0x8002a50c;break;} // 9421ffc0 stwu 1, -64(1)
case 0x8002a50c:{r[0]=c.lr;c.pc=0x8002a510;break;} // 7c0802a6 mflr 0
case 0x8002a510:{if(!store((r[1]+uint32_t(68)),r[0]))return {};c.pc=0x8002a514;break;} // 90010044 stw 0, 68(1)
case 0x8002a514:{while(c.step<11){const auto index=21+c.step;if(!store((r[1]+uint32_t(20))+4*c.step,r[index]))return {};++c.step;}c.step=0;c.pc=0x8002a518;break;} // bea10014 stmw 21, 20(1)
case 0x8002a518:{r[30]=r[7];c.pc=0x8002a51c;break;} // 7cfe3b78 mr 30, 7
case 0x8002a51c:{r[29]=r[6];c.pc=0x8002a520;break;} // 7cdd3378 mr 29, 6
case 0x8002a520:{r[26]=r[3];c.pc=0x8002a524;break;} // 7c7a1b78 mr 26, 3
case 0x8002a524:{r[27]=r[4];c.pc=0x8002a528;break;} // 7c9b2378 mr 27, 4
case 0x8002a528:{r[28]=r[5];c.pc=0x8002a52c;break;} // 7cbc2b78 mr 28, 5
case 0x8002a52c:{r[6]=r[30];c.pc=0x8002a530;break;} // 7fc6f378 mr 6, 30
case 0x8002a530:{c.lr=0x8002a534;c.pc=0x8002a780;break;} // 48000251 bl 0x8002a780
case 0x8002a534:{if(!word((r[26]+uint32_t(12)),r[22]))return {};c.pc=0x8002a538;break;} // 82da000c lwz 22, 12(26)
case 0x8002a538:{r[31]=r[3];c.pc=0x8002a53c;break;} // 7c7f1b78 mr 31, 3
case 0x8002a53c:{compare(c,0,int64_t(r[22]),int64_t(0));c.pc=0x8002a540;break;} // 28160000 cmplwi 22, 0
case 0x8002a540:{c.pc=c.condition[2]?0x8002a64c:0x8002a544;break;} // 4182010c bt 2, 0x8002a64c
case 0x8002a544:{r[3]=r[22];c.pc=0x8002a548;break;} // 7ec3b378 mr 3, 22
case 0x8002a548:{r[4]=r[31];c.pc=0x8002a54c;break;} // 7fe4fb78 mr 4, 31
case 0x8002a54c:{r[6]=r[30];c.pc=0x8002a550;break;} // 7fc6f378 mr 6, 30
case 0x8002a550:{r[5]=r[31]+uint32_t(28);c.pc=0x8002a554;break;} // 38bf001c addi 5, 31, 28
case 0x8002a554:{c.lr=0x8002a558;c.pc=0x8002a780;break;} // 4800022d bl 0x8002a780
case 0x8002a558:{if(!word((r[22]+uint32_t(12)),r[21]))return {};c.pc=0x8002a55c;break;} // 82b6000c lwz 21, 12(22)
case 0x8002a55c:{r[25]=r[3];c.pc=0x8002a560;break;} // 7c791b78 mr 25, 3
case 0x8002a560:{compare(c,0,int64_t(r[21]),int64_t(0));c.pc=0x8002a564;break;} // 28150000 cmplwi 21, 0
case 0x8002a564:{c.pc=c.condition[2]?0x8002a5d4:0x8002a568;break;} // 41820070 bt 2, 0x8002a5d4
case 0x8002a568:{r[23]=r[25]+uint32_t(28);c.pc=0x8002a56c;break;} // 3af9001c addi 23, 25, 28
case 0x8002a56c:{r[3]=r[21];c.pc=0x8002a570;break;} // 7ea3ab78 mr 3, 21
case 0x8002a570:{r[4]=r[25];c.pc=0x8002a574;break;} // 7f24cb78 mr 4, 25
case 0x8002a574:{r[6]=r[30];c.pc=0x8002a578;break;} // 7fc6f378 mr 6, 30
case 0x8002a578:{r[5]=r[23];c.pc=0x8002a57c;break;} // 7ee5bb78 mr 5, 23
case 0x8002a57c:{c.lr=0x8002a580;c.pc=0x8002a780;break;} // 48000205 bl 0x8002a780
case 0x8002a580:{if(!word((r[21]+uint32_t(12)),r[0]))return {};c.pc=0x8002a584;break;} // 8015000c lwz 0, 12(21)
case 0x8002a584:{r[24]=r[3];c.pc=0x8002a588;break;} // 7c781b78 mr 24, 3
case 0x8002a588:{compare(c,0,int64_t(r[0]),int64_t(0));c.pc=0x8002a58c;break;} // 28000000 cmplwi 0, 0
case 0x8002a58c:{c.pc=c.condition[2]?0x8002a5ac:0x8002a590;break;} // 41820020 bt 2, 0x8002a5ac
case 0x8002a590:{r[3]=r[0];c.pc=0x8002a594;break;} // 7c030378 mr 3, 0
case 0x8002a594:{r[4]=r[24];c.pc=0x8002a598;break;} // 7f04c378 mr 4, 24
case 0x8002a598:{r[7]=r[30];c.pc=0x8002a59c;break;} // 7fc7f378 mr 7, 30
case 0x8002a59c:{r[5]=r[24]+uint32_t(28);c.pc=0x8002a5a0;break;} // 38b8001c addi 5, 24, 28
case 0x8002a5a0:{r[6]=uint32_t(0);c.pc=0x8002a5a4;break;} // 38c00000 li 6, 0
case 0x8002a5a4:{c.lr=0x8002a5a8;c.pc=0x8002a508;break;} // 4bffff65 bl 0x8002a508
case 0x8002a5a8:{if(!store((r[24]+uint32_t(228)),r[3]))return {};c.pc=0x8002a5ac;break;} // 907800e4 stw 3, 228(24)
case 0x8002a5ac:{if(!word((r[21]+uint32_t(16)),r[3]))return {};c.pc=0x8002a5b0;break;} // 80750010 lwz 3, 16(21)
case 0x8002a5b0:{compare(c,0,int64_t(r[3]),int64_t(0));c.pc=0x8002a5b4;break;} // 28030000 cmplwi 3, 0
case 0x8002a5b4:{c.pc=c.condition[2]?0x8002a5d0:0x8002a5b8;break;} // 4182001c bt 2, 0x8002a5d0
case 0x8002a5b8:{r[4]=r[25];c.pc=0x8002a5bc;break;} // 7f24cb78 mr 4, 25
case 0x8002a5bc:{r[5]=r[23];c.pc=0x8002a5c0;break;} // 7ee5bb78 mr 5, 23
case 0x8002a5c0:{r[7]=r[30];c.pc=0x8002a5c4;break;} // 7fc7f378 mr 7, 30
case 0x8002a5c4:{r[6]=uint32_t(0);c.pc=0x8002a5c8;break;} // 38c00000 li 6, 0
case 0x8002a5c8:{c.lr=0x8002a5cc;c.pc=0x8002a508;break;} // 4bffff41 bl 0x8002a508
case 0x8002a5cc:{if(!store((r[24]+uint32_t(232)),r[3]))return {};c.pc=0x8002a5d0;break;} // 907800e8 stw 3, 232(24)
case 0x8002a5d0:{if(!store((r[25]+uint32_t(228)),r[24]))return {};c.pc=0x8002a5d4;break;} // 931900e4 stw 24, 228(25)
case 0x8002a5d4:{if(!word((r[22]+uint32_t(16)),r[21]))return {};c.pc=0x8002a5d8;break;} // 82b60010 lwz 21, 16(22)
case 0x8002a5d8:{compare(c,0,int64_t(r[21]),int64_t(0));c.pc=0x8002a5dc;break;} // 28150000 cmplwi 21, 0
case 0x8002a5dc:{c.pc=c.condition[2]?0x8002a648:0x8002a5e0;break;} // 4182006c bt 2, 0x8002a648
case 0x8002a5e0:{r[3]=r[21];c.pc=0x8002a5e4;break;} // 7ea3ab78 mr 3, 21
case 0x8002a5e4:{r[4]=r[31];c.pc=0x8002a5e8;break;} // 7fe4fb78 mr 4, 31
case 0x8002a5e8:{r[6]=r[30];c.pc=0x8002a5ec;break;} // 7fc6f378 mr 6, 30
case 0x8002a5ec:{r[5]=r[31]+uint32_t(28);c.pc=0x8002a5f0;break;} // 38bf001c addi 5, 31, 28
case 0x8002a5f0:{c.lr=0x8002a5f4;c.pc=0x8002a780;break;} // 48000191 bl 0x8002a780
case 0x8002a5f4:{if(!word((r[21]+uint32_t(12)),r[0]))return {};c.pc=0x8002a5f8;break;} // 8015000c lwz 0, 12(21)
case 0x8002a5f8:{r[23]=r[3];c.pc=0x8002a5fc;break;} // 7c771b78 mr 23, 3
case 0x8002a5fc:{compare(c,0,int64_t(r[0]),int64_t(0));c.pc=0x8002a600;break;} // 28000000 cmplwi 0, 0
case 0x8002a600:{c.pc=c.condition[2]?0x8002a620:0x8002a604;break;} // 41820020 bt 2, 0x8002a620
case 0x8002a604:{r[3]=r[0];c.pc=0x8002a608;break;} // 7c030378 mr 3, 0
case 0x8002a608:{r[4]=r[23];c.pc=0x8002a60c;break;} // 7ee4bb78 mr 4, 23
case 0x8002a60c:{r[7]=r[30];c.pc=0x8002a610;break;} // 7fc7f378 mr 7, 30
case 0x8002a610:{r[5]=r[23]+uint32_t(28);c.pc=0x8002a614;break;} // 38b7001c addi 5, 23, 28
case 0x8002a614:{r[6]=uint32_t(0);c.pc=0x8002a618;break;} // 38c00000 li 6, 0
case 0x8002a618:{c.lr=0x8002a61c;c.pc=0x8002a508;break;} // 4bfffef1 bl 0x8002a508
case 0x8002a61c:{if(!store((r[23]+uint32_t(228)),r[3]))return {};c.pc=0x8002a620;break;} // 907700e4 stw 3, 228(23)
case 0x8002a620:{if(!word((r[21]+uint32_t(16)),r[3]))return {};c.pc=0x8002a624;break;} // 80750010 lwz 3, 16(21)
case 0x8002a624:{compare(c,0,int64_t(r[3]),int64_t(0));c.pc=0x8002a628;break;} // 28030000 cmplwi 3, 0
case 0x8002a628:{c.pc=c.condition[2]?0x8002a644:0x8002a62c;break;} // 4182001c bt 2, 0x8002a644
case 0x8002a62c:{r[4]=r[31];c.pc=0x8002a630;break;} // 7fe4fb78 mr 4, 31
case 0x8002a630:{r[7]=r[30];c.pc=0x8002a634;break;} // 7fc7f378 mr 7, 30
case 0x8002a634:{r[5]=r[31]+uint32_t(28);c.pc=0x8002a638;break;} // 38bf001c addi 5, 31, 28
case 0x8002a638:{r[6]=uint32_t(0);c.pc=0x8002a63c;break;} // 38c00000 li 6, 0
case 0x8002a63c:{c.lr=0x8002a640;c.pc=0x8002a508;break;} // 4bfffecd bl 0x8002a508
case 0x8002a640:{if(!store((r[23]+uint32_t(232)),r[3]))return {};c.pc=0x8002a644;break;} // 907700e8 stw 3, 232(23)
case 0x8002a644:{if(!store((r[25]+uint32_t(232)),r[23]))return {};c.pc=0x8002a648;break;} // 92f900e8 stw 23, 232(25)
case 0x8002a648:{if(!store((r[31]+uint32_t(228)),r[25]))return {};c.pc=0x8002a64c;break;} // 933f00e4 stw 25, 228(31)
case 0x8002a64c:{compare(c,0,signedWord(r[29]),int64_t(0));c.pc=0x8002a650;break;} // 2c1d0000 cmpwi 29, 0
case 0x8002a650:{c.pc=!c.condition[2]?0x8002a768:0x8002a654;break;} // 40820118 bf 2, 0x8002a768
case 0x8002a654:{if(!word((r[26]+uint32_t(16)),r[21]))return {};c.pc=0x8002a658;break;} // 82ba0010 lwz 21, 16(26)
case 0x8002a658:{compare(c,0,int64_t(r[21]),int64_t(0));c.pc=0x8002a65c;break;} // 28150000 cmplwi 21, 0
case 0x8002a65c:{c.pc=c.condition[2]?0x8002a768:0x8002a660;break;} // 4182010c bt 2, 0x8002a768
case 0x8002a660:{r[3]=r[21];c.pc=0x8002a664;break;} // 7ea3ab78 mr 3, 21
case 0x8002a664:{r[4]=r[27];c.pc=0x8002a668;break;} // 7f64db78 mr 4, 27
case 0x8002a668:{r[5]=r[28];c.pc=0x8002a66c;break;} // 7f85e378 mr 5, 28
case 0x8002a66c:{r[6]=r[30];c.pc=0x8002a670;break;} // 7fc6f378 mr 6, 30
case 0x8002a670:{c.lr=0x8002a674;c.pc=0x8002a780;break;} // 48000111 bl 0x8002a780
case 0x8002a674:{if(!word((r[21]+uint32_t(12)),r[22]))return {};c.pc=0x8002a678;break;} // 82d5000c lwz 22, 12(21)
case 0x8002a678:{r[25]=r[3];c.pc=0x8002a67c;break;} // 7c791b78 mr 25, 3
case 0x8002a67c:{compare(c,0,int64_t(r[22]),int64_t(0));c.pc=0x8002a680;break;} // 28160000 cmplwi 22, 0
case 0x8002a680:{c.pc=c.condition[2]?0x8002a6f0:0x8002a684;break;} // 41820070 bt 2, 0x8002a6f0
case 0x8002a684:{r[24]=r[25]+uint32_t(28);c.pc=0x8002a688;break;} // 3b19001c addi 24, 25, 28
case 0x8002a688:{r[3]=r[22];c.pc=0x8002a68c;break;} // 7ec3b378 mr 3, 22
case 0x8002a68c:{r[4]=r[25];c.pc=0x8002a690;break;} // 7f24cb78 mr 4, 25
case 0x8002a690:{r[6]=r[30];c.pc=0x8002a694;break;} // 7fc6f378 mr 6, 30
case 0x8002a694:{r[5]=r[24];c.pc=0x8002a698;break;} // 7f05c378 mr 5, 24
case 0x8002a698:{c.lr=0x8002a69c;c.pc=0x8002a780;break;} // 480000e9 bl 0x8002a780
case 0x8002a69c:{if(!word((r[22]+uint32_t(12)),r[0]))return {};c.pc=0x8002a6a0;break;} // 8016000c lwz 0, 12(22)
case 0x8002a6a0:{r[23]=r[3];c.pc=0x8002a6a4;break;} // 7c771b78 mr 23, 3
case 0x8002a6a4:{compare(c,0,int64_t(r[0]),int64_t(0));c.pc=0x8002a6a8;break;} // 28000000 cmplwi 0, 0
case 0x8002a6a8:{c.pc=c.condition[2]?0x8002a6c8:0x8002a6ac;break;} // 41820020 bt 2, 0x8002a6c8
case 0x8002a6ac:{r[3]=r[0];c.pc=0x8002a6b0;break;} // 7c030378 mr 3, 0
case 0x8002a6b0:{r[4]=r[23];c.pc=0x8002a6b4;break;} // 7ee4bb78 mr 4, 23
case 0x8002a6b4:{r[7]=r[30];c.pc=0x8002a6b8;break;} // 7fc7f378 mr 7, 30
case 0x8002a6b8:{r[5]=r[23]+uint32_t(28);c.pc=0x8002a6bc;break;} // 38b7001c addi 5, 23, 28
case 0x8002a6bc:{r[6]=uint32_t(0);c.pc=0x8002a6c0;break;} // 38c00000 li 6, 0
case 0x8002a6c0:{c.lr=0x8002a6c4;c.pc=0x8002a508;break;} // 4bfffe49 bl 0x8002a508
case 0x8002a6c4:{if(!store((r[23]+uint32_t(228)),r[3]))return {};c.pc=0x8002a6c8;break;} // 907700e4 stw 3, 228(23)
case 0x8002a6c8:{if(!word((r[22]+uint32_t(16)),r[3]))return {};c.pc=0x8002a6cc;break;} // 80760010 lwz 3, 16(22)
case 0x8002a6cc:{compare(c,0,int64_t(r[3]),int64_t(0));c.pc=0x8002a6d0;break;} // 28030000 cmplwi 3, 0
case 0x8002a6d0:{c.pc=c.condition[2]?0x8002a6ec:0x8002a6d4;break;} // 4182001c bt 2, 0x8002a6ec
case 0x8002a6d4:{r[4]=r[25];c.pc=0x8002a6d8;break;} // 7f24cb78 mr 4, 25
case 0x8002a6d8:{r[5]=r[24];c.pc=0x8002a6dc;break;} // 7f05c378 mr 5, 24
case 0x8002a6dc:{r[7]=r[30];c.pc=0x8002a6e0;break;} // 7fc7f378 mr 7, 30
case 0x8002a6e0:{r[6]=uint32_t(0);c.pc=0x8002a6e4;break;} // 38c00000 li 6, 0
case 0x8002a6e4:{c.lr=0x8002a6e8;c.pc=0x8002a508;break;} // 4bfffe25 bl 0x8002a508
case 0x8002a6e8:{if(!store((r[23]+uint32_t(232)),r[3]))return {};c.pc=0x8002a6ec;break;} // 907700e8 stw 3, 232(23)
case 0x8002a6ec:{if(!store((r[25]+uint32_t(228)),r[23]))return {};c.pc=0x8002a6f0;break;} // 92f900e4 stw 23, 228(25)
case 0x8002a6f0:{if(!word((r[21]+uint32_t(16)),r[21]))return {};c.pc=0x8002a6f4;break;} // 82b50010 lwz 21, 16(21)
case 0x8002a6f4:{compare(c,0,int64_t(r[21]),int64_t(0));c.pc=0x8002a6f8;break;} // 28150000 cmplwi 21, 0
case 0x8002a6f8:{c.pc=c.condition[2]?0x8002a764:0x8002a6fc;break;} // 4182006c bt 2, 0x8002a764
case 0x8002a6fc:{r[3]=r[21];c.pc=0x8002a700;break;} // 7ea3ab78 mr 3, 21
case 0x8002a700:{r[4]=r[27];c.pc=0x8002a704;break;} // 7f64db78 mr 4, 27
case 0x8002a704:{r[5]=r[28];c.pc=0x8002a708;break;} // 7f85e378 mr 5, 28
case 0x8002a708:{r[6]=r[30];c.pc=0x8002a70c;break;} // 7fc6f378 mr 6, 30
case 0x8002a70c:{c.lr=0x8002a710;c.pc=0x8002a780;break;} // 48000075 bl 0x8002a780
case 0x8002a710:{if(!word((r[21]+uint32_t(12)),r[0]))return {};c.pc=0x8002a714;break;} // 8015000c lwz 0, 12(21)
case 0x8002a714:{r[23]=r[3];c.pc=0x8002a718;break;} // 7c771b78 mr 23, 3
case 0x8002a718:{compare(c,0,int64_t(r[0]),int64_t(0));c.pc=0x8002a71c;break;} // 28000000 cmplwi 0, 0
case 0x8002a71c:{c.pc=c.condition[2]?0x8002a73c:0x8002a720;break;} // 41820020 bt 2, 0x8002a73c
case 0x8002a720:{r[3]=r[0];c.pc=0x8002a724;break;} // 7c030378 mr 3, 0
case 0x8002a724:{r[4]=r[23];c.pc=0x8002a728;break;} // 7ee4bb78 mr 4, 23
case 0x8002a728:{r[7]=r[30];c.pc=0x8002a72c;break;} // 7fc7f378 mr 7, 30
case 0x8002a72c:{r[5]=r[23]+uint32_t(28);c.pc=0x8002a730;break;} // 38b7001c addi 5, 23, 28
case 0x8002a730:{r[6]=uint32_t(0);c.pc=0x8002a734;break;} // 38c00000 li 6, 0
case 0x8002a734:{c.lr=0x8002a738;c.pc=0x8002a508;break;} // 4bfffdd5 bl 0x8002a508
case 0x8002a738:{if(!store((r[23]+uint32_t(228)),r[3]))return {};c.pc=0x8002a73c;break;} // 907700e4 stw 3, 228(23)
case 0x8002a73c:{if(!word((r[21]+uint32_t(16)),r[3]))return {};c.pc=0x8002a740;break;} // 80750010 lwz 3, 16(21)
case 0x8002a740:{compare(c,0,int64_t(r[3]),int64_t(0));c.pc=0x8002a744;break;} // 28030000 cmplwi 3, 0
case 0x8002a744:{c.pc=c.condition[2]?0x8002a760:0x8002a748;break;} // 4182001c bt 2, 0x8002a760
case 0x8002a748:{r[4]=r[27];c.pc=0x8002a74c;break;} // 7f64db78 mr 4, 27
case 0x8002a74c:{r[5]=r[28];c.pc=0x8002a750;break;} // 7f85e378 mr 5, 28
case 0x8002a750:{r[7]=r[30];c.pc=0x8002a754;break;} // 7fc7f378 mr 7, 30
case 0x8002a754:{r[6]=uint32_t(0);c.pc=0x8002a758;break;} // 38c00000 li 6, 0
case 0x8002a758:{c.lr=0x8002a75c;c.pc=0x8002a508;break;} // 4bfffdb1 bl 0x8002a508
case 0x8002a75c:{if(!store((r[23]+uint32_t(232)),r[3]))return {};c.pc=0x8002a760;break;} // 907700e8 stw 3, 232(23)
case 0x8002a760:{if(!store((r[25]+uint32_t(232)),r[23]))return {};c.pc=0x8002a764;break;} // 92f900e8 stw 23, 232(25)
case 0x8002a764:{if(!store((r[31]+uint32_t(232)),r[25]))return {};c.pc=0x8002a768;break;} // 933f00e8 stw 25, 232(31)
case 0x8002a768:{r[3]=r[31];c.pc=0x8002a76c;break;} // 7fe3fb78 mr 3, 31
case 0x8002a76c:{while(c.step<11){const auto index=21+c.step;if(!word((r[1]+uint32_t(20))+4*c.step,r[index]))return {};++c.step;}c.step=0;c.pc=0x8002a770;break;} // baa10014 lmw 21, 20(1)
case 0x8002a770:{if(!word((r[1]+uint32_t(68)),r[0]))return {};c.pc=0x8002a774;break;} // 80010044 lwz 0, 68(1)
case 0x8002a774:{c.lr=r[0];c.pc=0x8002a778;break;} // 7c0803a6 mtlr 0
case 0x8002a778:{r[1]=r[1]+uint32_t(64);c.pc=0x8002a77c;break;} // 38210040 addi 1, 1, 64
case 0x8002a77c:{c.pc=c.lr;break;} // 4e800020 blr 
case 0x8002b120:{const auto target=(r[1]+uint32_t(-16));if(!store(target,r[1]))return {};r[1]=target;c.pc=0x8002b124;break;} // 9421fff0 stwu 1, -16(1)
case 0x8002b124:{r[0]=c.lr;c.pc=0x8002b128;break;} // 7c0802a6 mflr 0
case 0x8002b128:{compare(c,0,int64_t(r[3]),int64_t(0));c.pc=0x8002b12c;break;} // 28030000 cmplwi 3, 0
case 0x8002b12c:{if(!store((r[1]+uint32_t(20)),r[0]))return {};c.pc=0x8002b130;break;} // 90010014 stw 0, 20(1)
case 0x8002b130:{r[0]=uint32_t(0);c.pc=0x8002b134;break;} // 38000000 li 0, 0
case 0x8002b134:{if(!store((r[1]+uint32_t(8)),r[0]))return {};c.pc=0x8002b138;break;} // 90010008 stw 0, 8(1)
case 0x8002b138:{c.pc=c.condition[2]?0x8002b158:0x8002b13c;break;} // 41820020 bt 2, 0x8002b158
case 0x8002b13c:{r[0]=uint32_t(1);c.pc=0x8002b140;break;} // 38000001 li 0, 1
case 0x8002b140:{if(!store((r[1]+uint32_t(8)),r[0]))return {};c.pc=0x8002b144;break;} // 90010008 stw 0, 8(1)
case 0x8002b144:{if(!word((r[3]+uint32_t(12)),r[3]))return {};c.pc=0x8002b148;break;} // 8063000c lwz 3, 12(3)
case 0x8002b148:{compare(c,0,int64_t(r[3]),int64_t(0));c.pc=0x8002b14c;break;} // 28030000 cmplwi 3, 0
case 0x8002b14c:{c.pc=c.condition[2]?0x8002b158:0x8002b150;break;} // 4182000c bt 2, 0x8002b158
case 0x8002b150:{r[4]=r[1]+uint32_t(8);c.pc=0x8002b154;break;} // 38810008 addi 4, 1, 8
case 0x8002b154:{c.lr=0x8002b158;c.pc=0x8002b16c;break;} // 48000019 bl 0x8002b16c
case 0x8002b158:{if(!word((r[1]+uint32_t(20)),r[0]))return {};c.pc=0x8002b15c;break;} // 80010014 lwz 0, 20(1)
case 0x8002b15c:{if(!word((r[1]+uint32_t(8)),r[3]))return {};c.pc=0x8002b160;break;} // 80610008 lwz 3, 8(1)
case 0x8002b160:{c.lr=r[0];c.pc=0x8002b164;break;} // 7c0803a6 mtlr 0
case 0x8002b164:{r[1]=r[1]+uint32_t(16);c.pc=0x8002b168;break;} // 38210010 addi 1, 1, 16
case 0x8002b168:{c.pc=c.lr;break;} // 4e800020 blr 
case 0x8002b16c:{const auto target=(r[1]+uint32_t(-32));if(!store(target,r[1]))return {};r[1]=target;c.pc=0x8002b170;break;} // 9421ffe0 stwu 1, -32(1)
case 0x8002b170:{r[0]=c.lr;c.pc=0x8002b174;break;} // 7c0802a6 mflr 0
case 0x8002b174:{if(!store((r[1]+uint32_t(36)),r[0]))return {};c.pc=0x8002b178;break;} // 90010024 stw 0, 36(1)
case 0x8002b178:{while(c.step<5){const auto index=27+c.step;if(!store((r[1]+uint32_t(12))+4*c.step,r[index]))return {};++c.step;}c.step=0;c.pc=0x8002b17c;break;} // bf61000c stmw 27, 12(1)
case 0x8002b17c:{r[30]=r[3];c.pc=0x8002b180;break;} // 7c7e1b78 mr 30, 3
case 0x8002b180:{r[31]=r[4];c.pc=0x8002b184;break;} // 7c9f2378 mr 31, 4
case 0x8002b184:{if(!word((r[4]+uint32_t(0)),r[3]))return {};c.pc=0x8002b188;break;} // 80640000 lwz 3, 0(4)
case 0x8002b188:{r[0]=r[3]+uint32_t(1);c.pc=0x8002b18c;break;} // 38030001 addi 0, 3, 1
case 0x8002b18c:{if(!store((r[4]+uint32_t(0)),r[0]))return {};c.pc=0x8002b190;break;} // 90040000 stw 0, 0(4)
case 0x8002b190:{if(!word((r[30]+uint32_t(12)),r[29]))return {};c.pc=0x8002b194;break;} // 83be000c lwz 29, 12(30)
case 0x8002b194:{compare(c,0,int64_t(r[29]),int64_t(0));c.pc=0x8002b198;break;} // 281d0000 cmplwi 29, 0
case 0x8002b198:{c.pc=c.condition[2]?0x8002b2d4:0x8002b19c;break;} // 4182013c bt 2, 0x8002b2d4
case 0x8002b19c:{if(!word((r[31]+uint32_t(0)),r[3]))return {};c.pc=0x8002b1a0;break;} // 807f0000 lwz 3, 0(31)
case 0x8002b1a0:{r[0]=r[3]+uint32_t(1);c.pc=0x8002b1a4;break;} // 38030001 addi 0, 3, 1
case 0x8002b1a4:{if(!store((r[31]+uint32_t(0)),r[0]))return {};c.pc=0x8002b1a8;break;} // 901f0000 stw 0, 0(31)
case 0x8002b1a8:{if(!word((r[29]+uint32_t(12)),r[28]))return {};c.pc=0x8002b1ac;break;} // 839d000c lwz 28, 12(29)
case 0x8002b1ac:{compare(c,0,int64_t(r[28]),int64_t(0));c.pc=0x8002b1b0;break;} // 281c0000 cmplwi 28, 0
case 0x8002b1b0:{c.pc=c.condition[2]?0x8002b23c:0x8002b1b4;break;} // 4182008c bt 2, 0x8002b23c
case 0x8002b1b4:{if(!word((r[31]+uint32_t(0)),r[3]))return {};c.pc=0x8002b1b8;break;} // 807f0000 lwz 3, 0(31)
case 0x8002b1b8:{r[0]=r[3]+uint32_t(1);c.pc=0x8002b1bc;break;} // 38030001 addi 0, 3, 1
case 0x8002b1bc:{if(!store((r[31]+uint32_t(0)),r[0]))return {};c.pc=0x8002b1c0;break;} // 901f0000 stw 0, 0(31)
case 0x8002b1c0:{if(!word((r[28]+uint32_t(12)),r[27]))return {};c.pc=0x8002b1c4;break;} // 837c000c lwz 27, 12(28)
case 0x8002b1c4:{compare(c,0,int64_t(r[27]),int64_t(0));c.pc=0x8002b1c8;break;} // 281b0000 cmplwi 27, 0
case 0x8002b1c8:{c.pc=c.condition[2]?0x8002b1fc:0x8002b1cc;break;} // 41820034 bt 2, 0x8002b1fc
case 0x8002b1cc:{if(!word((r[31]+uint32_t(0)),r[3]))return {};c.pc=0x8002b1d0;break;} // 807f0000 lwz 3, 0(31)
case 0x8002b1d0:{r[0]=r[3]+uint32_t(1);c.pc=0x8002b1d4;break;} // 38030001 addi 0, 3, 1
case 0x8002b1d4:{if(!store((r[31]+uint32_t(0)),r[0]))return {};c.pc=0x8002b1d8;break;} // 901f0000 stw 0, 0(31)
case 0x8002b1d8:{if(!word((r[27]+uint32_t(12)),r[3]))return {};c.pc=0x8002b1dc;break;} // 807b000c lwz 3, 12(27)
case 0x8002b1dc:{compare(c,0,int64_t(r[3]),int64_t(0));c.pc=0x8002b1e0;break;} // 28030000 cmplwi 3, 0
case 0x8002b1e0:{c.pc=c.condition[2]?0x8002b1e8:0x8002b1e4;break;} // 41820008 bt 2, 0x8002b1e8
case 0x8002b1e4:{c.lr=0x8002b1e8;c.pc=0x8002b16c;break;} // 4bffff89 bl 0x8002b16c
case 0x8002b1e8:{if(!word((r[27]+uint32_t(16)),r[3]))return {};c.pc=0x8002b1ec;break;} // 807b0010 lwz 3, 16(27)
case 0x8002b1ec:{compare(c,0,int64_t(r[3]),int64_t(0));c.pc=0x8002b1f0;break;} // 28030000 cmplwi 3, 0
case 0x8002b1f0:{c.pc=c.condition[2]?0x8002b1fc:0x8002b1f4;break;} // 4182000c bt 2, 0x8002b1fc
case 0x8002b1f4:{r[4]=r[31];c.pc=0x8002b1f8;break;} // 7fe4fb78 mr 4, 31
case 0x8002b1f8:{c.lr=0x8002b1fc;c.pc=0x8002b16c;break;} // 4bffff75 bl 0x8002b16c
case 0x8002b1fc:{if(!word((r[28]+uint32_t(16)),r[27]))return {};c.pc=0x8002b200;break;} // 837c0010 lwz 27, 16(28)
case 0x8002b200:{compare(c,0,int64_t(r[27]),int64_t(0));c.pc=0x8002b204;break;} // 281b0000 cmplwi 27, 0
case 0x8002b204:{c.pc=c.condition[2]?0x8002b23c:0x8002b208;break;} // 41820038 bt 2, 0x8002b23c
case 0x8002b208:{if(!word((r[31]+uint32_t(0)),r[3]))return {};c.pc=0x8002b20c;break;} // 807f0000 lwz 3, 0(31)
case 0x8002b20c:{r[0]=r[3]+uint32_t(1);c.pc=0x8002b210;break;} // 38030001 addi 0, 3, 1
case 0x8002b210:{if(!store((r[31]+uint32_t(0)),r[0]))return {};c.pc=0x8002b214;break;} // 901f0000 stw 0, 0(31)
case 0x8002b214:{if(!word((r[27]+uint32_t(12)),r[3]))return {};c.pc=0x8002b218;break;} // 807b000c lwz 3, 12(27)
case 0x8002b218:{compare(c,0,int64_t(r[3]),int64_t(0));c.pc=0x8002b21c;break;} // 28030000 cmplwi 3, 0
case 0x8002b21c:{c.pc=c.condition[2]?0x8002b228:0x8002b220;break;} // 4182000c bt 2, 0x8002b228
case 0x8002b220:{r[4]=r[31];c.pc=0x8002b224;break;} // 7fe4fb78 mr 4, 31
case 0x8002b224:{c.lr=0x8002b228;c.pc=0x8002b16c;break;} // 4bffff49 bl 0x8002b16c
case 0x8002b228:{if(!word((r[27]+uint32_t(16)),r[3]))return {};c.pc=0x8002b22c;break;} // 807b0010 lwz 3, 16(27)
case 0x8002b22c:{compare(c,0,int64_t(r[3]),int64_t(0));c.pc=0x8002b230;break;} // 28030000 cmplwi 3, 0
case 0x8002b230:{c.pc=c.condition[2]?0x8002b23c:0x8002b234;break;} // 4182000c bt 2, 0x8002b23c
case 0x8002b234:{r[4]=r[31];c.pc=0x8002b238;break;} // 7fe4fb78 mr 4, 31
case 0x8002b238:{c.lr=0x8002b23c;c.pc=0x8002b16c;break;} // 4bffff35 bl 0x8002b16c
case 0x8002b23c:{if(!word((r[29]+uint32_t(16)),r[27]))return {};c.pc=0x8002b240;break;} // 837d0010 lwz 27, 16(29)
case 0x8002b240:{compare(c,0,int64_t(r[27]),int64_t(0));c.pc=0x8002b244;break;} // 281b0000 cmplwi 27, 0
case 0x8002b244:{c.pc=c.condition[2]?0x8002b2d4:0x8002b248;break;} // 41820090 bt 2, 0x8002b2d4
case 0x8002b248:{if(!word((r[31]+uint32_t(0)),r[3]))return {};c.pc=0x8002b24c;break;} // 807f0000 lwz 3, 0(31)
case 0x8002b24c:{r[0]=r[3]+uint32_t(1);c.pc=0x8002b250;break;} // 38030001 addi 0, 3, 1
case 0x8002b250:{if(!store((r[31]+uint32_t(0)),r[0]))return {};c.pc=0x8002b254;break;} // 901f0000 stw 0, 0(31)
case 0x8002b254:{if(!word((r[27]+uint32_t(12)),r[28]))return {};c.pc=0x8002b258;break;} // 839b000c lwz 28, 12(27)
case 0x8002b258:{compare(c,0,int64_t(r[28]),int64_t(0));c.pc=0x8002b25c;break;} // 281c0000 cmplwi 28, 0
case 0x8002b25c:{c.pc=c.condition[2]?0x8002b294:0x8002b260;break;} // 41820038 bt 2, 0x8002b294
case 0x8002b260:{if(!word((r[31]+uint32_t(0)),r[3]))return {};c.pc=0x8002b264;break;} // 807f0000 lwz 3, 0(31)
case 0x8002b264:{r[0]=r[3]+uint32_t(1);c.pc=0x8002b268;break;} // 38030001 addi 0, 3, 1
case 0x8002b268:{if(!store((r[31]+uint32_t(0)),r[0]))return {};c.pc=0x8002b26c;break;} // 901f0000 stw 0, 0(31)
case 0x8002b26c:{if(!word((r[28]+uint32_t(12)),r[3]))return {};c.pc=0x8002b270;break;} // 807c000c lwz 3, 12(28)
case 0x8002b270:{compare(c,0,int64_t(r[3]),int64_t(0));c.pc=0x8002b274;break;} // 28030000 cmplwi 3, 0
case 0x8002b274:{c.pc=c.condition[2]?0x8002b280:0x8002b278;break;} // 4182000c bt 2, 0x8002b280
case 0x8002b278:{r[4]=r[31];c.pc=0x8002b27c;break;} // 7fe4fb78 mr 4, 31
case 0x8002b27c:{c.lr=0x8002b280;c.pc=0x8002b16c;break;} // 4bfffef1 bl 0x8002b16c
case 0x8002b280:{if(!word((r[28]+uint32_t(16)),r[3]))return {};c.pc=0x8002b284;break;} // 807c0010 lwz 3, 16(28)
case 0x8002b284:{compare(c,0,int64_t(r[3]),int64_t(0));c.pc=0x8002b288;break;} // 28030000 cmplwi 3, 0
case 0x8002b288:{c.pc=c.condition[2]?0x8002b294:0x8002b28c;break;} // 4182000c bt 2, 0x8002b294
case 0x8002b28c:{r[4]=r[31];c.pc=0x8002b290;break;} // 7fe4fb78 mr 4, 31
case 0x8002b290:{c.lr=0x8002b294;c.pc=0x8002b16c;break;} // 4bfffedd bl 0x8002b16c
case 0x8002b294:{if(!word((r[27]+uint32_t(16)),r[27]))return {};c.pc=0x8002b298;break;} // 837b0010 lwz 27, 16(27)
case 0x8002b298:{compare(c,0,int64_t(r[27]),int64_t(0));c.pc=0x8002b29c;break;} // 281b0000 cmplwi 27, 0
case 0x8002b29c:{c.pc=c.condition[2]?0x8002b2d4:0x8002b2a0;break;} // 41820038 bt 2, 0x8002b2d4
case 0x8002b2a0:{if(!word((r[31]+uint32_t(0)),r[3]))return {};c.pc=0x8002b2a4;break;} // 807f0000 lwz 3, 0(31)
case 0x8002b2a4:{r[0]=r[3]+uint32_t(1);c.pc=0x8002b2a8;break;} // 38030001 addi 0, 3, 1
case 0x8002b2a8:{if(!store((r[31]+uint32_t(0)),r[0]))return {};c.pc=0x8002b2ac;break;} // 901f0000 stw 0, 0(31)
case 0x8002b2ac:{if(!word((r[27]+uint32_t(12)),r[3]))return {};c.pc=0x8002b2b0;break;} // 807b000c lwz 3, 12(27)
case 0x8002b2b0:{compare(c,0,int64_t(r[3]),int64_t(0));c.pc=0x8002b2b4;break;} // 28030000 cmplwi 3, 0
case 0x8002b2b4:{c.pc=c.condition[2]?0x8002b2c0:0x8002b2b8;break;} // 4182000c bt 2, 0x8002b2c0
case 0x8002b2b8:{r[4]=r[31];c.pc=0x8002b2bc;break;} // 7fe4fb78 mr 4, 31
case 0x8002b2bc:{c.lr=0x8002b2c0;c.pc=0x8002b16c;break;} // 4bfffeb1 bl 0x8002b16c
case 0x8002b2c0:{if(!word((r[27]+uint32_t(16)),r[3]))return {};c.pc=0x8002b2c4;break;} // 807b0010 lwz 3, 16(27)
case 0x8002b2c4:{compare(c,0,int64_t(r[3]),int64_t(0));c.pc=0x8002b2c8;break;} // 28030000 cmplwi 3, 0
case 0x8002b2c8:{c.pc=c.condition[2]?0x8002b2d4:0x8002b2cc;break;} // 4182000c bt 2, 0x8002b2d4
case 0x8002b2cc:{r[4]=r[31];c.pc=0x8002b2d0;break;} // 7fe4fb78 mr 4, 31
case 0x8002b2d0:{c.lr=0x8002b2d4;c.pc=0x8002b16c;break;} // 4bfffe9d bl 0x8002b16c
case 0x8002b2d4:{if(!word((r[30]+uint32_t(16)),r[27]))return {};c.pc=0x8002b2d8;break;} // 837e0010 lwz 27, 16(30)
case 0x8002b2d8:{compare(c,0,int64_t(r[27]),int64_t(0));c.pc=0x8002b2dc;break;} // 281b0000 cmplwi 27, 0
case 0x8002b2dc:{c.pc=c.condition[2]?0x8002b41c:0x8002b2e0;break;} // 41820140 bt 2, 0x8002b41c
case 0x8002b2e0:{if(!word((r[31]+uint32_t(0)),r[3]))return {};c.pc=0x8002b2e4;break;} // 807f0000 lwz 3, 0(31)
case 0x8002b2e4:{r[0]=r[3]+uint32_t(1);c.pc=0x8002b2e8;break;} // 38030001 addi 0, 3, 1
case 0x8002b2e8:{if(!store((r[31]+uint32_t(0)),r[0]))return {};c.pc=0x8002b2ec;break;} // 901f0000 stw 0, 0(31)
case 0x8002b2ec:{if(!word((r[27]+uint32_t(12)),r[28]))return {};c.pc=0x8002b2f0;break;} // 839b000c lwz 28, 12(27)
case 0x8002b2f0:{compare(c,0,int64_t(r[28]),int64_t(0));c.pc=0x8002b2f4;break;} // 281c0000 cmplwi 28, 0
case 0x8002b2f4:{c.pc=c.condition[2]?0x8002b384:0x8002b2f8;break;} // 41820090 bt 2, 0x8002b384
case 0x8002b2f8:{if(!word((r[31]+uint32_t(0)),r[3]))return {};c.pc=0x8002b2fc;break;} // 807f0000 lwz 3, 0(31)
case 0x8002b2fc:{r[0]=r[3]+uint32_t(1);c.pc=0x8002b300;break;} // 38030001 addi 0, 3, 1
case 0x8002b300:{if(!store((r[31]+uint32_t(0)),r[0]))return {};c.pc=0x8002b304;break;} // 901f0000 stw 0, 0(31)
case 0x8002b304:{if(!word((r[28]+uint32_t(12)),r[29]))return {};c.pc=0x8002b308;break;} // 83bc000c lwz 29, 12(28)
case 0x8002b308:{compare(c,0,int64_t(r[29]),int64_t(0));c.pc=0x8002b30c;break;} // 281d0000 cmplwi 29, 0
case 0x8002b30c:{c.pc=c.condition[2]?0x8002b344:0x8002b310;break;} // 41820038 bt 2, 0x8002b344
case 0x8002b310:{if(!word((r[31]+uint32_t(0)),r[3]))return {};c.pc=0x8002b314;break;} // 807f0000 lwz 3, 0(31)
case 0x8002b314:{r[0]=r[3]+uint32_t(1);c.pc=0x8002b318;break;} // 38030001 addi 0, 3, 1
case 0x8002b318:{if(!store((r[31]+uint32_t(0)),r[0]))return {};c.pc=0x8002b31c;break;} // 901f0000 stw 0, 0(31)
case 0x8002b31c:{if(!word((r[29]+uint32_t(12)),r[3]))return {};c.pc=0x8002b320;break;} // 807d000c lwz 3, 12(29)
case 0x8002b320:{compare(c,0,int64_t(r[3]),int64_t(0));c.pc=0x8002b324;break;} // 28030000 cmplwi 3, 0
case 0x8002b324:{c.pc=c.condition[2]?0x8002b330:0x8002b328;break;} // 4182000c bt 2, 0x8002b330
case 0x8002b328:{r[4]=r[31];c.pc=0x8002b32c;break;} // 7fe4fb78 mr 4, 31
case 0x8002b32c:{c.lr=0x8002b330;c.pc=0x8002b16c;break;} // 4bfffe41 bl 0x8002b16c
case 0x8002b330:{if(!word((r[29]+uint32_t(16)),r[3]))return {};c.pc=0x8002b334;break;} // 807d0010 lwz 3, 16(29)
case 0x8002b334:{compare(c,0,int64_t(r[3]),int64_t(0));c.pc=0x8002b338;break;} // 28030000 cmplwi 3, 0
case 0x8002b338:{c.pc=c.condition[2]?0x8002b344:0x8002b33c;break;} // 4182000c bt 2, 0x8002b344
case 0x8002b33c:{r[4]=r[31];c.pc=0x8002b340;break;} // 7fe4fb78 mr 4, 31
case 0x8002b340:{c.lr=0x8002b344;c.pc=0x8002b16c;break;} // 4bfffe2d bl 0x8002b16c
case 0x8002b344:{if(!word((r[28]+uint32_t(16)),r[28]))return {};c.pc=0x8002b348;break;} // 839c0010 lwz 28, 16(28)
case 0x8002b348:{compare(c,0,int64_t(r[28]),int64_t(0));c.pc=0x8002b34c;break;} // 281c0000 cmplwi 28, 0
case 0x8002b34c:{c.pc=c.condition[2]?0x8002b384:0x8002b350;break;} // 41820038 bt 2, 0x8002b384
case 0x8002b350:{if(!word((r[31]+uint32_t(0)),r[3]))return {};c.pc=0x8002b354;break;} // 807f0000 lwz 3, 0(31)
case 0x8002b354:{r[0]=r[3]+uint32_t(1);c.pc=0x8002b358;break;} // 38030001 addi 0, 3, 1
case 0x8002b358:{if(!store((r[31]+uint32_t(0)),r[0]))return {};c.pc=0x8002b35c;break;} // 901f0000 stw 0, 0(31)
case 0x8002b35c:{if(!word((r[28]+uint32_t(12)),r[3]))return {};c.pc=0x8002b360;break;} // 807c000c lwz 3, 12(28)
case 0x8002b360:{compare(c,0,int64_t(r[3]),int64_t(0));c.pc=0x8002b364;break;} // 28030000 cmplwi 3, 0
case 0x8002b364:{c.pc=c.condition[2]?0x8002b370:0x8002b368;break;} // 4182000c bt 2, 0x8002b370
case 0x8002b368:{r[4]=r[31];c.pc=0x8002b36c;break;} // 7fe4fb78 mr 4, 31
case 0x8002b36c:{c.lr=0x8002b370;c.pc=0x8002b16c;break;} // 4bfffe01 bl 0x8002b16c
case 0x8002b370:{if(!word((r[28]+uint32_t(16)),r[3]))return {};c.pc=0x8002b374;break;} // 807c0010 lwz 3, 16(28)
case 0x8002b374:{compare(c,0,int64_t(r[3]),int64_t(0));c.pc=0x8002b378;break;} // 28030000 cmplwi 3, 0
case 0x8002b378:{c.pc=c.condition[2]?0x8002b384:0x8002b37c;break;} // 4182000c bt 2, 0x8002b384
case 0x8002b37c:{r[4]=r[31];c.pc=0x8002b380;break;} // 7fe4fb78 mr 4, 31
case 0x8002b380:{c.lr=0x8002b384;c.pc=0x8002b16c;break;} // 4bfffded bl 0x8002b16c
case 0x8002b384:{if(!word((r[27]+uint32_t(16)),r[27]))return {};c.pc=0x8002b388;break;} // 837b0010 lwz 27, 16(27)
case 0x8002b388:{compare(c,0,int64_t(r[27]),int64_t(0));c.pc=0x8002b38c;break;} // 281b0000 cmplwi 27, 0
case 0x8002b38c:{c.pc=c.condition[2]?0x8002b41c:0x8002b390;break;} // 41820090 bt 2, 0x8002b41c
case 0x8002b390:{if(!word((r[31]+uint32_t(0)),r[3]))return {};c.pc=0x8002b394;break;} // 807f0000 lwz 3, 0(31)
case 0x8002b394:{r[0]=r[3]+uint32_t(1);c.pc=0x8002b398;break;} // 38030001 addi 0, 3, 1
case 0x8002b398:{if(!store((r[31]+uint32_t(0)),r[0]))return {};c.pc=0x8002b39c;break;} // 901f0000 stw 0, 0(31)
case 0x8002b39c:{if(!word((r[27]+uint32_t(12)),r[28]))return {};c.pc=0x8002b3a0;break;} // 839b000c lwz 28, 12(27)
case 0x8002b3a0:{compare(c,0,int64_t(r[28]),int64_t(0));c.pc=0x8002b3a4;break;} // 281c0000 cmplwi 28, 0
case 0x8002b3a4:{c.pc=c.condition[2]?0x8002b3dc:0x8002b3a8;break;} // 41820038 bt 2, 0x8002b3dc
case 0x8002b3a8:{if(!word((r[31]+uint32_t(0)),r[3]))return {};c.pc=0x8002b3ac;break;} // 807f0000 lwz 3, 0(31)
case 0x8002b3ac:{r[0]=r[3]+uint32_t(1);c.pc=0x8002b3b0;break;} // 38030001 addi 0, 3, 1
case 0x8002b3b0:{if(!store((r[31]+uint32_t(0)),r[0]))return {};c.pc=0x8002b3b4;break;} // 901f0000 stw 0, 0(31)
case 0x8002b3b4:{if(!word((r[28]+uint32_t(12)),r[3]))return {};c.pc=0x8002b3b8;break;} // 807c000c lwz 3, 12(28)
case 0x8002b3b8:{compare(c,0,int64_t(r[3]),int64_t(0));c.pc=0x8002b3bc;break;} // 28030000 cmplwi 3, 0
case 0x8002b3bc:{c.pc=c.condition[2]?0x8002b3c8:0x8002b3c0;break;} // 4182000c bt 2, 0x8002b3c8
case 0x8002b3c0:{r[4]=r[31];c.pc=0x8002b3c4;break;} // 7fe4fb78 mr 4, 31
case 0x8002b3c4:{c.lr=0x8002b3c8;c.pc=0x8002b16c;break;} // 4bfffda9 bl 0x8002b16c
case 0x8002b3c8:{if(!word((r[28]+uint32_t(16)),r[3]))return {};c.pc=0x8002b3cc;break;} // 807c0010 lwz 3, 16(28)
case 0x8002b3cc:{compare(c,0,int64_t(r[3]),int64_t(0));c.pc=0x8002b3d0;break;} // 28030000 cmplwi 3, 0
case 0x8002b3d0:{c.pc=c.condition[2]?0x8002b3dc:0x8002b3d4;break;} // 4182000c bt 2, 0x8002b3dc
case 0x8002b3d4:{r[4]=r[31];c.pc=0x8002b3d8;break;} // 7fe4fb78 mr 4, 31
case 0x8002b3d8:{c.lr=0x8002b3dc;c.pc=0x8002b16c;break;} // 4bfffd95 bl 0x8002b16c
case 0x8002b3dc:{if(!word((r[27]+uint32_t(16)),r[27]))return {};c.pc=0x8002b3e0;break;} // 837b0010 lwz 27, 16(27)
case 0x8002b3e0:{compare(c,0,int64_t(r[27]),int64_t(0));c.pc=0x8002b3e4;break;} // 281b0000 cmplwi 27, 0
case 0x8002b3e4:{c.pc=c.condition[2]?0x8002b41c:0x8002b3e8;break;} // 41820038 bt 2, 0x8002b41c
case 0x8002b3e8:{if(!word((r[31]+uint32_t(0)),r[3]))return {};c.pc=0x8002b3ec;break;} // 807f0000 lwz 3, 0(31)
case 0x8002b3ec:{r[0]=r[3]+uint32_t(1);c.pc=0x8002b3f0;break;} // 38030001 addi 0, 3, 1
case 0x8002b3f0:{if(!store((r[31]+uint32_t(0)),r[0]))return {};c.pc=0x8002b3f4;break;} // 901f0000 stw 0, 0(31)
case 0x8002b3f4:{if(!word((r[27]+uint32_t(12)),r[3]))return {};c.pc=0x8002b3f8;break;} // 807b000c lwz 3, 12(27)
case 0x8002b3f8:{compare(c,0,int64_t(r[3]),int64_t(0));c.pc=0x8002b3fc;break;} // 28030000 cmplwi 3, 0
case 0x8002b3fc:{c.pc=c.condition[2]?0x8002b408:0x8002b400;break;} // 4182000c bt 2, 0x8002b408
case 0x8002b400:{r[4]=r[31];c.pc=0x8002b404;break;} // 7fe4fb78 mr 4, 31
case 0x8002b404:{c.lr=0x8002b408;c.pc=0x8002b16c;break;} // 4bfffd69 bl 0x8002b16c
case 0x8002b408:{if(!word((r[27]+uint32_t(16)),r[3]))return {};c.pc=0x8002b40c;break;} // 807b0010 lwz 3, 16(27)
case 0x8002b40c:{compare(c,0,int64_t(r[3]),int64_t(0));c.pc=0x8002b410;break;} // 28030000 cmplwi 3, 0
case 0x8002b410:{c.pc=c.condition[2]?0x8002b41c:0x8002b414;break;} // 4182000c bt 2, 0x8002b41c
case 0x8002b414:{r[4]=r[31];c.pc=0x8002b418;break;} // 7fe4fb78 mr 4, 31
case 0x8002b418:{c.lr=0x8002b41c;c.pc=0x8002b16c;break;} // 4bfffd55 bl 0x8002b16c
case 0x8002b41c:{while(c.step<5){const auto index=27+c.step;if(!word((r[1]+uint32_t(12))+4*c.step,r[index]))return {};++c.step;}c.step=0;c.pc=0x8002b420;break;} // bb61000c lmw 27, 12(1)
case 0x8002b420:{if(!word((r[1]+uint32_t(36)),r[0]))return {};c.pc=0x8002b424;break;} // 80010024 lwz 0, 36(1)
case 0x8002b424:{c.lr=r[0];c.pc=0x8002b428;break;} // 7c0803a6 mtlr 0
case 0x8002b428:{r[1]=r[1]+uint32_t(32);c.pc=0x8002b42c;break;} // 38210020 addi 1, 1, 32
case 0x8002b42c:{c.pc=c.lr;break;} // 4e800020 blr 
case 0x8002a780:{const auto target=(r[1]+uint32_t(-288));if(!store(target,r[1]))return {};r[1]=target;c.pc=0x8002a784;break;} // 9421fee0 stwu 1, -288(1)
case 0x8002a784:{r[0]=c.lr;c.pc=0x8002a788;break;} // 7c0802a6 mflr 0
case 0x8002a788:{r[8]=r[6]*uint32_t(376);c.pc=0x8002a78c;break;} // 1d060178 mulli 8, 6, 376
case 0x8002a78c:{if(!store((r[1]+uint32_t(292)),r[0]))return {};c.pc=0x8002a790;break;} // 90010124 stw 0, 292(1)
case 0x8002a790:{if(!store((r[1]+uint32_t(284)),r[31]))return {};c.pc=0x8002a794;break;} // 93e1011c stw 31, 284(1)
case 0x8002a794:{if(!store((r[1]+uint32_t(280)),r[30]))return {};c.pc=0x8002a798;break;} // 93c10118 stw 30, 280(1)
case 0x8002a798:{r[30]=r[4];c.pc=0x8002a79c;break;} // 7c9e2378 mr 30, 4
case 0x8002a79c:{if(!store((r[1]+uint32_t(276)),r[29]))return {};c.pc=0x8002a7a0;break;} // 93a10114 stw 29, 276(1)
case 0x8002a7a0:{r[29]=r[3];c.pc=0x8002a7a4;break;} // 7c7d1b78 mr 29, 3
case 0x8002a7a4:{if(!store((r[1]+uint32_t(272)),r[28]))return {};c.pc=0x8002a7a8;break;} // 93810110 stw 28, 272(1)
case 0x8002a7a8:{r[28]=r[5];c.pc=0x8002a7ac;break;} // 7cbc2b78 mr 28, 5
case 0x8002a7ac:{if(!word((r[13]+uint32_t(5956)),r[0]))return {};c.pc=0x8002a7b0;break;} // 800d1744 lwz 0, 5956(13)
case 0x8002a7b0:{if(!word((r[13]+uint32_t(5952)),r[7]))return {};c.pc=0x8002a7b4;break;} // 80ed1740 lwz 7, 5952(13)
case 0x8002a7b4:{r[3]=r[0]*uint32_t(756);c.pc=0x8002a7b8;break;} // 1c6002f4 mulli 3, 0, 756
case 0x8002a7b8:{r[0]=r[8]+uint32_t(336);c.pc=0x8002a7bc;break;} // 38080150 addi 0, 8, 336
case 0x8002a7bc:{r[4]=r[7]+r[3];c.pc=0x8002a7c0;break;} // 7c871a14 add 4, 7, 3
case 0x8002a7c0:{if(!word(r[4]+r[0],r[0]))return {};c.pc=0x8002a7c4;break;} // 7c04002e lwzx 0, 4, 0
case 0x8002a7c4:{r[3]=r[4]+r[8];c.pc=0x8002a7c8;break;} // 7c644214 add 3, 4, 8
case 0x8002a7c8:{if(!word((r[3]+uint32_t(340)),r[31]))return {};c.pc=0x8002a7cc;break;} // 83e30154 lwz 31, 340(3)
case 0x8002a7cc:{c.ctr=r[0];c.pc=0x8002a7d0;break;} // 7c0903a6 mtctr 0
case 0x8002a7d0:{compare(c,0,signedWord(r[0]),int64_t(0));c.pc=0x8002a7d4;break;} // 2c000000 cmpwi 0, 0
case 0x8002a7d4:{c.pc=!c.condition[1]?0x8002a7ec:0x8002a7d8;break;} // 40810018 bf 1, 0x8002a7ec
case 0x8002a7d8:{if(!word((r[31]+uint32_t(8)),r[0]))return {};c.pc=0x8002a7dc;break;} // 801f0008 lwz 0, 8(31)
case 0x8002a7dc:{compare(c,0,int64_t(r[0]),int64_t(0));c.pc=0x8002a7e0;break;} // 28000000 cmplwi 0, 0
case 0x8002a7e0:{c.pc=c.condition[2]?0x8002a7ec:0x8002a7e4;break;} // 4182000c bt 2, 0x8002a7ec
case 0x8002a7e4:{r[31]=r[31]+uint32_t(308);c.pc=0x8002a7e8;break;} // 3bff0134 addi 31, 31, 308
case 0x8002a7e8:{c.pc=--c.ctr?0x8002a7d8:0x8002a7ec;break;} // 4200fff0 bdnz 0x8002a7d8
case 0x8002a7ec:{r[0]=uint32_t(0);c.pc=0x8002a7f0;break;} // 38000000 li 0, 0
case 0x8002a7f0:{r[3]=r[1]+uint32_t(224);c.pc=0x8002a7f4;break;} // 386100e0 addi 3, 1, 224
case 0x8002a7f4:{if(!store((r[31]+uint32_t(0)),r[0]))return {};c.pc=0x8002a7f8;break;} // 901f0000 stw 0, 0(31)
case 0x8002a7f8:{if(!store((r[31]+uint32_t(8)),r[29]))return {};c.pc=0x8002a7fc;break;} // 93bf0008 stw 29, 8(31)
case 0x8002a7fc:{if(!s.storeHalf || !s.storeHalf(s.user,(r[31]+uint32_t(222)),uint16_t(r[6])))return {};c.pc=0x8002a800;break;} // b0df00de sth 6, 222(31)
case 0x8002a800:{if(!word((r[29]+uint32_t(48)),f[1]))return {};c.pc=0x8002a804;break;} // c03d0030 lfs 1, 48(29)
case 0x8002a804:{if(!word((r[29]+uint32_t(52)),f[2]))return {};c.pc=0x8002a808;break;} // c05d0034 lfs 2, 52(29)
case 0x8002a808:{if(!word((r[29]+uint32_t(56)),f[3]))return {};c.pc=0x8002a80c;break;} // c07d0038 lfs 3, 56(29)
case 0x8002a80c:{if(!external(0x8029e230,0x8002a810))return {};break;} // 48273a25 bl 0x8029e230
case 0x8002a810:{if(!word((r[29]+uint32_t(24)),f[1]))return {};c.pc=0x8002a814;break;} // c03d0018 lfs 1, 24(29)
case 0x8002a814:{r[3]=r[1]+uint32_t(176);c.pc=0x8002a818;break;} // 386100b0 addi 3, 1, 176
case 0x8002a818:{if(!word((r[29]+uint32_t(28)),f[2]))return {};c.pc=0x8002a81c;break;} // c05d001c lfs 2, 28(29)
case 0x8002a81c:{if(!word((r[29]+uint32_t(32)),f[3]))return {};c.pc=0x8002a820;break;} // c07d0020 lfs 3, 32(29)
case 0x8002a820:{if(!external(0x8029e2b0,0x8002a824))return {};break;} // 48273a91 bl 0x8029e2b0
case 0x8002a824:{if(!word((r[2]+uint32_t(-31480)),f[1]))return {};c.pc=0x8002a828;break;} // c0228508 lfs 1, -31480(2)
case 0x8002a828:{r[3]=r[1]+uint32_t(128);c.pc=0x8002a82c;break;} // 38610080 addi 3, 1, 128
case 0x8002a82c:{if(!word((r[29]+uint32_t(36)),f[0]))return {};c.pc=0x8002a830;break;} // c01d0024 lfs 0, 36(29)
case 0x8002a830:{r[4]=uint32_t(120);c.pc=0x8002a834;break;} // 38800078 li 4, 120
case 0x8002a834:{f[1]=bits(scalar(f[1])*scalar(f[0]));c.pc=0x8002a838;break;} // ec210032 fmuls 1, 1, 0
case 0x8002a838:{if(!external(0x8029dff0,0x8002a83c))return {};break;} // 482737b9 bl 0x8029dff0
case 0x8002a83c:{if(!word((r[2]+uint32_t(-31480)),f[1]))return {};c.pc=0x8002a840;break;} // c0228508 lfs 1, -31480(2)
case 0x8002a840:{r[3]=r[1]+uint32_t(80);c.pc=0x8002a844;break;} // 38610050 addi 3, 1, 80
case 0x8002a844:{if(!word((r[29]+uint32_t(40)),f[0]))return {};c.pc=0x8002a848;break;} // c01d0028 lfs 0, 40(29)
case 0x8002a848:{r[4]=uint32_t(121);c.pc=0x8002a84c;break;} // 38800079 li 4, 121
case 0x8002a84c:{f[1]=bits(scalar(f[1])*scalar(f[0]));c.pc=0x8002a850;break;} // ec210032 fmuls 1, 1, 0
case 0x8002a850:{if(!external(0x8029dff0,0x8002a854))return {};break;} // 482737a1 bl 0x8029dff0
case 0x8002a854:{if(!word((r[2]+uint32_t(-31480)),f[1]))return {};c.pc=0x8002a858;break;} // c0228508 lfs 1, -31480(2)
case 0x8002a858:{r[3]=r[1]+uint32_t(32);c.pc=0x8002a85c;break;} // 38610020 addi 3, 1, 32
case 0x8002a85c:{if(!word((r[29]+uint32_t(44)),f[0]))return {};c.pc=0x8002a860;break;} // c01d002c lfs 0, 44(29)
case 0x8002a860:{r[4]=uint32_t(122);c.pc=0x8002a864;break;} // 3880007a li 4, 122
case 0x8002a864:{f[1]=bits(scalar(f[1])*scalar(f[0]));c.pc=0x8002a868;break;} // ec210032 fmuls 1, 1, 0
case 0x8002a868:{if(!external(0x8029dff0,0x8002a86c))return {};break;} // 48273789 bl 0x8029dff0
case 0x8002a86c:{r[3]=r[1]+uint32_t(224);c.pc=0x8002a870;break;} // 386100e0 addi 3, 1, 224
case 0x8002a870:{r[4]=r[1]+uint32_t(32);c.pc=0x8002a874;break;} // 38810020 addi 4, 1, 32
case 0x8002a874:{r[5]=r[3];c.pc=0x8002a878;break;} // 7c651b78 mr 5, 3
case 0x8002a878:{if(!external(0x8029dd64,0x8002a87c))return {};break;} // 482734ed bl 0x8029dd64
case 0x8002a87c:{r[3]=r[1]+uint32_t(224);c.pc=0x8002a880;break;} // 386100e0 addi 3, 1, 224
case 0x8002a880:{r[4]=r[1]+uint32_t(80);c.pc=0x8002a884;break;} // 38810050 addi 4, 1, 80
case 0x8002a884:{r[5]=r[3];c.pc=0x8002a888;break;} // 7c651b78 mr 5, 3
case 0x8002a888:{if(!external(0x8029dd64,0x8002a88c))return {};break;} // 482734dd bl 0x8029dd64
case 0x8002a88c:{r[3]=r[1]+uint32_t(224);c.pc=0x8002a890;break;} // 386100e0 addi 3, 1, 224
case 0x8002a890:{r[4]=r[1]+uint32_t(128);c.pc=0x8002a894;break;} // 38810080 addi 4, 1, 128
case 0x8002a894:{r[5]=r[3];c.pc=0x8002a898;break;} // 7c651b78 mr 5, 3
case 0x8002a898:{if(!external(0x8029dd64,0x8002a89c))return {};break;} // 482734cd bl 0x8029dd64
case 0x8002a89c:{r[3]=r[1]+uint32_t(224);c.pc=0x8002a8a0;break;} // 386100e0 addi 3, 1, 224
case 0x8002a8a0:{r[4]=r[1]+uint32_t(176);c.pc=0x8002a8a4;break;} // 388100b0 addi 4, 1, 176
case 0x8002a8a4:{r[5]=r[31]+uint32_t(76);c.pc=0x8002a8a8;break;} // 38bf004c addi 5, 31, 76
case 0x8002a8a8:{if(!external(0x8029dd64,0x8002a8ac))return {};break;} // 482734bd bl 0x8029dd64
case 0x8002a8ac:{r[3]=r[28];c.pc=0x8002a8b0;break;} // 7f83e378 mr 3, 28
case 0x8002a8b0:{r[4]=r[31]+uint32_t(76);c.pc=0x8002a8b4;break;} // 389f004c addi 4, 31, 76
case 0x8002a8b4:{r[5]=r[31]+uint32_t(28);c.pc=0x8002a8b8;break;} // 38bf001c addi 5, 31, 28
case 0x8002a8b8:{if(!external(0x8029dd64,0x8002a8bc))return {};break;} // 482734ad bl 0x8029dd64
case 0x8002a8bc:{r[3]=r[31]+uint32_t(172);c.pc=0x8002a8c0;break;} // 387f00ac addi 3, 31, 172
case 0x8002a8c0:{if(!external(0x8029dd04,0x8002a8c4))return {};break;} // 48273445 bl 0x8029dd04
case 0x8002a8c4:{if(!word((r[31]+uint32_t(0)),r[5]))return {};c.pc=0x8002a8c8;break;} // 80bf0000 lwz 5, 0(31)
case 0x8002a8c8:{r[4]=uint32_t(4);c.pc=0x8002a8cc;break;} // 38800004 li 4, 4
case 0x8002a8cc:{r[3]=uint32_t(-1);c.pc=0x8002a8d0;break;} // 3860ffff li 3, -1
case 0x8002a8d0:{r[0]=uint32_t(0);c.pc=0x8002a8d4;break;} // 38000000 li 0, 0
case 0x8002a8d4:{r[5]=r[5]|(uint32_t(512));c.pc=0x8002a8d8;break;} // 60a50200 ori 5, 5, 512
case 0x8002a8d8:{if(!store((r[31]+uint32_t(0)),r[5]))return {};c.pc=0x8002a8dc;break;} // 90bf0000 stw 5, 0(31)
case 0x8002a8dc:{if(!s.storeByte || !s.storeByte(s.user,(r[31]+uint32_t(5)),uint8_t(r[4])))return {};c.pc=0x8002a8e0;break;} // 989f0005 stb 4, 5(31)
case 0x8002a8e0:{if(!s.storeHalf || !s.storeHalf(s.user,(r[31]+uint32_t(220)),uint16_t(r[3])))return {};c.pc=0x8002a8e4;break;} // b07f00dc sth 3, 220(31)
case 0x8002a8e4:{if(!store((r[31]+uint32_t(224)),r[30]))return {};c.pc=0x8002a8e8;break;} // 93df00e0 stw 30, 224(31)
case 0x8002a8e8:{if(!store((r[31]+uint32_t(232)),r[0]))return {};c.pc=0x8002a8ec;break;} // 901f00e8 stw 0, 232(31)
case 0x8002a8ec:{if(!store((r[31]+uint32_t(228)),r[0]))return {};c.pc=0x8002a8f0;break;} // 901f00e4 stw 0, 228(31)
case 0x8002a8f0:{if(!word((r[29]+uint32_t(88)),r[3]))return {};c.pc=0x8002a8f4;break;} // 807d0058 lwz 3, 88(29)
case 0x8002a8f4:{compare(c,0,int64_t(r[3]),int64_t(0));c.pc=0x8002a8f8;break;} // 28030000 cmplwi 3, 0
case 0x8002a8f8:{c.pc=c.condition[2]?0x8002ac30:0x8002a8fc;break;} // 41820338 bt 2, 0x8002ac30
case 0x8002a8fc:{if(!byte((r[3]+uint32_t(2)),r[0]))return {};c.pc=0x8002a900;break;} // 88030002 lbz 0, 2(3)
case 0x8002a900:{r[0]=r[0]&0x00000001u;compare(c,0,signedWord(r[0]),0);c.pc=0x8002a904;break;} // 540007ff clrlwi. 0, 0, 31
case 0x8002a904:{c.pc=c.condition[2]?0x8002a914:0x8002a908;break;} // 41820010 bt 2, 0x8002a914
case 0x8002a908:{if(!word((r[31]+uint32_t(0)),r[0]))return {};c.pc=0x8002a90c;break;} // 801f0000 lwz 0, 0(31)
case 0x8002a90c:{r[0]=r[0]|(uint32_t(8));c.pc=0x8002a910;break;} // 60000008 ori 0, 0, 8
case 0x8002a910:{if(!store((r[31]+uint32_t(0)),r[0]))return {};c.pc=0x8002a914;break;} // 901f0000 stw 0, 0(31)
case 0x8002a914:{if(!word((r[29]+uint32_t(88)),r[3]))return {};c.pc=0x8002a918;break;} // 807d0058 lwz 3, 88(29)
case 0x8002a918:{if(!byte((r[3]+uint32_t(0)),r[0]))return {};c.pc=0x8002a91c;break;} // 88030000 lbz 0, 0(3)
case 0x8002a91c:{compare(c,0,signedWord(r[0]),int64_t(2));c.pc=0x8002a920;break;} // 2c000002 cmpwi 0, 2
case 0x8002a920:{c.pc=c.condition[2]?0x8002aad8:0x8002a924;break;} // 418201b8 bt 2, 0x8002aad8
case 0x8002a924:{c.pc=!c.condition[0]?0x8002a938:0x8002a928;break;} // 40800014 bf 0, 0x8002a938
case 0x8002a928:{compare(c,0,signedWord(r[0]),int64_t(0));c.pc=0x8002a92c;break;} // 2c000000 cmpwi 0, 0
case 0x8002a92c:{c.pc=c.condition[2]?0x8002a944:0x8002a930;break;} // 41820018 bt 2, 0x8002a944
case 0x8002a930:{c.pc=!c.condition[0]?0x8002aabc:0x8002a934;break;} // 4080018c bf 0, 0x8002aabc
case 0x8002a934:{c.pc=0x8002ab0c;break;} // 480001d8 b 0x8002ab0c
case 0x8002a938:{compare(c,0,signedWord(r[0]),int64_t(4));c.pc=0x8002a93c;break;} // 2c000004 cmpwi 0, 4
case 0x8002a93c:{c.pc=!c.condition[0]?0x8002ab0c:0x8002a940;break;} // 408001d0 bf 0, 0x8002ab0c
case 0x8002a940:{c.pc=0x8002aaf4;break;} // 480001b4 b 0x8002aaf4
case 0x8002a944:{if(!word((r[31]+uint32_t(8)),r[8]))return {};c.pc=0x8002a948;break;} // 811f0008 lwz 8, 8(31)
case 0x8002a948:{r[9]=uint32_t(0);c.pc=0x8002a94c;break;} // 39200000 li 9, 0
case 0x8002a94c:{r[6]=r[9];c.pc=0x8002a950;break;} // 7d264b78 mr 6, 9
case 0x8002a950:{r[11]=uint32_t(0);c.pc=0x8002a954;break;} // 39600000 li 11, 0
case 0x8002a954:{if(!word((r[8]+uint32_t(92)),r[7]))return {};c.pc=0x8002a958;break;} // 80e8005c lwz 7, 92(8)
case 0x8002a958:{r[12]=uint32_t(0);c.pc=0x8002a95c;break;} // 39800000 li 12, 0
case 0x8002a95c:{c.pc=0x8002aa80;break;} // 48000124 b 0x8002aa80
case 0x8002a960:{r[0]=r[6]+uint32_t(96);c.pc=0x8002a964;break;} // 38060060 addi 0, 6, 96
case 0x8002a964:{if(!word(r[8]+r[0],r[3]))return {};c.pc=0x8002a968;break;} // 7c68002e lwzx 3, 8, 0
case 0x8002a968:{if(!byte((r[3]+uint32_t(10)),r[0]))return {};c.pc=0x8002a96c;break;} // 8803000a lbz 0, 10(3)
case 0x8002a96c:{compare(c,0,int64_t(r[0]),int64_t(2));c.pc=0x8002a970;break;} // 28000002 cmplwi 0, 2
case 0x8002a970:{c.pc=!c.condition[2]?0x8002a97c:0x8002a974;break;} // 4082000c bf 2, 0x8002a97c
case 0x8002a974:{r[11]=r[11]+uint32_t(1);c.pc=0x8002a978;break;} // 396b0001 addi 11, 11, 1
case 0x8002a978:{c.pc=0x8002a988;break;} // 48000010 b 0x8002a988
case 0x8002a97c:{compare(c,0,int64_t(r[0]),int64_t(1));c.pc=0x8002a980;break;} // 28000001 cmplwi 0, 1
case 0x8002a980:{c.pc=!c.condition[2]?0x8002a988:0x8002a984;break;} // 40820008 bf 2, 0x8002a988
case 0x8002a984:{r[12]=r[12]+uint32_t(1);c.pc=0x8002a988;break;} // 398c0001 addi 12, 12, 1
case 0x8002a988:{r[0]=uint32_t(2);c.pc=0x8002a98c;break;} // 38000002 li 0, 2
case 0x8002a98c:{r[4]=r[6]+uint32_t(96);c.pc=0x8002a990;break;} // 38860060 addi 4, 6, 96
case 0x8002a990:{r[10]=uint32_t(0);c.pc=0x8002a994;break;} // 39400000 li 10, 0
case 0x8002a994:{r[5]=uint32_t(0);c.pc=0x8002a998;break;} // 38a00000 li 5, 0
case 0x8002a998:{c.ctr=r[0];c.pc=0x8002a99c;break;} // 7c0903a6 mtctr 0
case 0x8002a99c:{if(!word(r[8]+r[4],r[3]))return {};c.pc=0x8002a9a0;break;} // 7c68202e lwzx 3, 8, 4
case 0x8002a9a0:{r[0]=r[5]+uint32_t(12);c.pc=0x8002a9a4;break;} // 3805000c addi 0, 5, 12
case 0x8002a9a4:{if(!word(r[3]+r[0],r[3]))return {};c.pc=0x8002a9a8;break;} // 7c63002e lwzx 3, 3, 0
case 0x8002a9a8:{compare(c,0,int64_t(r[3]),int64_t(0));c.pc=0x8002a9ac;break;} // 28030000 cmplwi 3, 0
case 0x8002a9ac:{c.pc=c.condition[2]?0x8002aa78:0x8002a9b0;break;} // 418200cc bt 2, 0x8002aa78
case 0x8002a9b0:{if(!byte((r[3]+uint32_t(10)),r[0]))return {};c.pc=0x8002a9b4;break;} // 8803000a lbz 0, 10(3)
case 0x8002a9b4:{compare(c,0,int64_t(r[0]),int64_t(2));c.pc=0x8002a9b8;break;} // 28000002 cmplwi 0, 2
case 0x8002a9b8:{c.pc=!c.condition[2]?0x8002a9c4:0x8002a9bc;break;} // 4082000c bf 2, 0x8002a9c4
case 0x8002a9bc:{r[11]=r[11]+uint32_t(1);c.pc=0x8002a9c0;break;} // 396b0001 addi 11, 11, 1
case 0x8002a9c0:{c.pc=0x8002a9d0;break;} // 48000010 b 0x8002a9d0
case 0x8002a9c4:{compare(c,0,int64_t(r[0]),int64_t(1));c.pc=0x8002a9c8;break;} // 28000001 cmplwi 0, 1
case 0x8002a9c8:{c.pc=!c.condition[2]?0x8002a9d0:0x8002a9cc;break;} // 40820008 bf 2, 0x8002a9d0
case 0x8002a9cc:{r[12]=r[12]+uint32_t(1);c.pc=0x8002a9d0;break;} // 398c0001 addi 12, 12, 1
case 0x8002a9d0:{if(!word(r[8]+r[4],r[3]))return {};c.pc=0x8002a9d4;break;} // 7c68202e lwzx 3, 8, 4
case 0x8002a9d4:{r[0]=r[5]+uint32_t(16);c.pc=0x8002a9d8;break;} // 38050010 addi 0, 5, 16
case 0x8002a9d8:{if(!word(r[3]+r[0],r[3]))return {};c.pc=0x8002a9dc;break;} // 7c63002e lwzx 3, 3, 0
case 0x8002a9dc:{compare(c,0,int64_t(r[3]),int64_t(0));c.pc=0x8002a9e0;break;} // 28030000 cmplwi 3, 0
case 0x8002a9e0:{c.pc=c.condition[2]?0x8002aa78:0x8002a9e4;break;} // 41820098 bt 2, 0x8002aa78
case 0x8002a9e4:{if(!byte((r[3]+uint32_t(10)),r[0]))return {};c.pc=0x8002a9e8;break;} // 8803000a lbz 0, 10(3)
case 0x8002a9e8:{compare(c,0,int64_t(r[0]),int64_t(2));c.pc=0x8002a9ec;break;} // 28000002 cmplwi 0, 2
case 0x8002a9ec:{c.pc=!c.condition[2]?0x8002a9f8:0x8002a9f0;break;} // 4082000c bf 2, 0x8002a9f8
case 0x8002a9f0:{r[11]=r[11]+uint32_t(1);c.pc=0x8002a9f4;break;} // 396b0001 addi 11, 11, 1
case 0x8002a9f4:{c.pc=0x8002aa04;break;} // 48000010 b 0x8002aa04
case 0x8002a9f8:{compare(c,0,int64_t(r[0]),int64_t(1));c.pc=0x8002a9fc;break;} // 28000001 cmplwi 0, 1
case 0x8002a9fc:{c.pc=!c.condition[2]?0x8002aa04:0x8002aa00;break;} // 40820008 bf 2, 0x8002aa04
case 0x8002aa00:{r[12]=r[12]+uint32_t(1);c.pc=0x8002aa04;break;} // 398c0001 addi 12, 12, 1
case 0x8002aa04:{if(!word(r[8]+r[4],r[3]))return {};c.pc=0x8002aa08;break;} // 7c68202e lwzx 3, 8, 4
case 0x8002aa08:{r[0]=r[5]+uint32_t(20);c.pc=0x8002aa0c;break;} // 38050014 addi 0, 5, 20
case 0x8002aa0c:{if(!word(r[3]+r[0],r[3]))return {};c.pc=0x8002aa10;break;} // 7c63002e lwzx 3, 3, 0
case 0x8002aa10:{compare(c,0,int64_t(r[3]),int64_t(0));c.pc=0x8002aa14;break;} // 28030000 cmplwi 3, 0
case 0x8002aa14:{c.pc=c.condition[2]?0x8002aa78:0x8002aa18;break;} // 41820064 bt 2, 0x8002aa78
case 0x8002aa18:{if(!byte((r[3]+uint32_t(10)),r[0]))return {};c.pc=0x8002aa1c;break;} // 8803000a lbz 0, 10(3)
case 0x8002aa1c:{compare(c,0,int64_t(r[0]),int64_t(2));c.pc=0x8002aa20;break;} // 28000002 cmplwi 0, 2
case 0x8002aa20:{c.pc=!c.condition[2]?0x8002aa2c:0x8002aa24;break;} // 4082000c bf 2, 0x8002aa2c
case 0x8002aa24:{r[11]=r[11]+uint32_t(1);c.pc=0x8002aa28;break;} // 396b0001 addi 11, 11, 1
case 0x8002aa28:{c.pc=0x8002aa38;break;} // 48000010 b 0x8002aa38
case 0x8002aa2c:{compare(c,0,int64_t(r[0]),int64_t(1));c.pc=0x8002aa30;break;} // 28000001 cmplwi 0, 1
case 0x8002aa30:{c.pc=!c.condition[2]?0x8002aa38:0x8002aa34;break;} // 40820008 bf 2, 0x8002aa38
case 0x8002aa34:{r[12]=r[12]+uint32_t(1);c.pc=0x8002aa38;break;} // 398c0001 addi 12, 12, 1
case 0x8002aa38:{if(!word(r[8]+r[4],r[3]))return {};c.pc=0x8002aa3c;break;} // 7c68202e lwzx 3, 8, 4
case 0x8002aa3c:{r[0]=r[5]+uint32_t(24);c.pc=0x8002aa40;break;} // 38050018 addi 0, 5, 24
case 0x8002aa40:{if(!word(r[3]+r[0],r[3]))return {};c.pc=0x8002aa44;break;} // 7c63002e lwzx 3, 3, 0
case 0x8002aa44:{compare(c,0,int64_t(r[3]),int64_t(0));c.pc=0x8002aa48;break;} // 28030000 cmplwi 3, 0
case 0x8002aa48:{c.pc=c.condition[2]?0x8002aa78:0x8002aa4c;break;} // 41820030 bt 2, 0x8002aa78
case 0x8002aa4c:{if(!byte((r[3]+uint32_t(10)),r[0]))return {};c.pc=0x8002aa50;break;} // 8803000a lbz 0, 10(3)
case 0x8002aa50:{compare(c,0,int64_t(r[0]),int64_t(2));c.pc=0x8002aa54;break;} // 28000002 cmplwi 0, 2
case 0x8002aa54:{c.pc=!c.condition[2]?0x8002aa60:0x8002aa58;break;} // 4082000c bf 2, 0x8002aa60
case 0x8002aa58:{r[11]=r[11]+uint32_t(1);c.pc=0x8002aa5c;break;} // 396b0001 addi 11, 11, 1
case 0x8002aa5c:{c.pc=0x8002aa6c;break;} // 48000010 b 0x8002aa6c
case 0x8002aa60:{compare(c,0,int64_t(r[0]),int64_t(1));c.pc=0x8002aa64;break;} // 28000001 cmplwi 0, 1
case 0x8002aa64:{c.pc=!c.condition[2]?0x8002aa6c:0x8002aa68;break;} // 40820008 bf 2, 0x8002aa6c
case 0x8002aa68:{r[12]=r[12]+uint32_t(1);c.pc=0x8002aa6c;break;} // 398c0001 addi 12, 12, 1
case 0x8002aa6c:{r[5]=r[5]+uint32_t(16);c.pc=0x8002aa70;break;} // 38a50010 addi 5, 5, 16
case 0x8002aa70:{r[10]=r[10]+uint32_t(3);c.pc=0x8002aa74;break;} // 394a0003 addi 10, 10, 3
case 0x8002aa74:{c.pc=--c.ctr?0x8002a99c:0x8002aa78;break;} // 4200ff28 bdnz 0x8002a99c
case 0x8002aa78:{r[6]=r[6]+uint32_t(8);c.pc=0x8002aa7c;break;} // 38c60008 addi 6, 6, 8
case 0x8002aa7c:{r[9]=r[9]+uint32_t(1);c.pc=0x8002aa80;break;} // 39290001 addi 9, 9, 1
case 0x8002aa80:{compare(c,0,signedWord(r[9]),signedWord(r[7]));c.pc=0x8002aa84;break;} // 7c093800 cmpw 9, 7
case 0x8002aa84:{c.pc=c.condition[0]?0x8002a960:0x8002aa88;break;} // 4180fedc bt 0, 0x8002a960
case 0x8002aa88:{compare(c,0,signedWord(r[11]),int64_t(0));c.pc=0x8002aa8c;break;} // 2c0b0000 cmpwi 11, 0
case 0x8002aa8c:{c.pc=c.condition[2]?0x8002aa9c:0x8002aa90;break;} // 41820010 bt 2, 0x8002aa9c
case 0x8002aa90:{r[0]=uint32_t(2);c.pc=0x8002aa94;break;} // 38000002 li 0, 2
case 0x8002aa94:{if(!s.storeByte || !s.storeByte(s.user,(r[31]+uint32_t(4)),uint8_t(r[0])))return {};c.pc=0x8002aa98;break;} // 981f0004 stb 0, 4(31)
case 0x8002aa98:{c.pc=0x8002ab0c;break;} // 48000074 b 0x8002ab0c
case 0x8002aa9c:{compare(c,0,signedWord(r[12]),int64_t(0));c.pc=0x8002aaa0;break;} // 2c0c0000 cmpwi 12, 0
case 0x8002aaa0:{c.pc=c.condition[2]?0x8002aab0:0x8002aaa4;break;} // 41820010 bt 2, 0x8002aab0
case 0x8002aaa4:{r[0]=uint32_t(1);c.pc=0x8002aaa8;break;} // 38000001 li 0, 1
case 0x8002aaa8:{if(!s.storeByte || !s.storeByte(s.user,(r[31]+uint32_t(4)),uint8_t(r[0])))return {};c.pc=0x8002aaac;break;} // 981f0004 stb 0, 4(31)
case 0x8002aaac:{c.pc=0x8002ab0c;break;} // 48000060 b 0x8002ab0c
case 0x8002aab0:{r[0]=uint32_t(0);c.pc=0x8002aab4;break;} // 38000000 li 0, 0
case 0x8002aab4:{if(!s.storeByte || !s.storeByte(s.user,(r[31]+uint32_t(4)),uint8_t(r[0])))return {};c.pc=0x8002aab8;break;} // 981f0004 stb 0, 4(31)
case 0x8002aab8:{c.pc=0x8002ab0c;break;} // 48000054 b 0x8002ab0c
case 0x8002aabc:{if(!word((r[31]+uint32_t(0)),r[0]))return {};c.pc=0x8002aac0;break;} // 801f0000 lwz 0, 0(31)
case 0x8002aac0:{r[3]=uint32_t(0);c.pc=0x8002aac4;break;} // 38600000 li 3, 0
case 0x8002aac4:{r[0]=rotate(r[0],0)&0x00000008u;compare(c,0,signedWord(r[0]),0);c.pc=0x8002aac8;break;} // 54000739 rlwinm. 0, 0, 0, 28, 28
case 0x8002aac8:{c.pc=c.condition[2]?0x8002aad0:0x8002aacc;break;} // 41820008 bt 2, 0x8002aad0
case 0x8002aacc:{r[3]=uint32_t(3);c.pc=0x8002aad0;break;} // 38600003 li 3, 3
case 0x8002aad0:{if(!s.storeByte || !s.storeByte(s.user,(r[31]+uint32_t(4)),uint8_t(r[3])))return {};c.pc=0x8002aad4;break;} // 987f0004 stb 3, 4(31)
case 0x8002aad4:{c.pc=0x8002ab0c;break;} // 48000038 b 0x8002ab0c
case 0x8002aad8:{if(!word((r[31]+uint32_t(0)),r[0]))return {};c.pc=0x8002aadc;break;} // 801f0000 lwz 0, 0(31)
case 0x8002aadc:{r[3]=uint32_t(1);c.pc=0x8002aae0;break;} // 38600001 li 3, 1
case 0x8002aae0:{r[0]=rotate(r[0],0)&0x00000008u;compare(c,0,signedWord(r[0]),0);c.pc=0x8002aae4;break;} // 54000739 rlwinm. 0, 0, 0, 28, 28
case 0x8002aae4:{c.pc=c.condition[2]?0x8002aaec:0x8002aae8;break;} // 41820008 bt 2, 0x8002aaec
case 0x8002aae8:{r[3]=uint32_t(4);c.pc=0x8002aaec;break;} // 38600004 li 3, 4
case 0x8002aaec:{if(!s.storeByte || !s.storeByte(s.user,(r[31]+uint32_t(4)),uint8_t(r[3])))return {};c.pc=0x8002aaf0;break;} // 987f0004 stb 3, 4(31)
case 0x8002aaf0:{c.pc=0x8002ab0c;break;} // 4800001c b 0x8002ab0c
case 0x8002aaf4:{if(!word((r[31]+uint32_t(0)),r[0]))return {};c.pc=0x8002aaf8;break;} // 801f0000 lwz 0, 0(31)
case 0x8002aaf8:{r[3]=uint32_t(2);c.pc=0x8002aafc;break;} // 38600002 li 3, 2
case 0x8002aafc:{r[0]=rotate(r[0],0)&0x00000008u;compare(c,0,signedWord(r[0]),0);c.pc=0x8002ab00;break;} // 54000739 rlwinm. 0, 0, 0, 28, 28
case 0x8002ab00:{c.pc=c.condition[2]?0x8002ab08:0x8002ab04;break;} // 41820008 bt 2, 0x8002ab08
case 0x8002ab04:{r[3]=uint32_t(5);c.pc=0x8002ab08;break;} // 38600005 li 3, 5
case 0x8002ab08:{if(!s.storeByte || !s.storeByte(s.user,(r[31]+uint32_t(4)),uint8_t(r[3])))return {};c.pc=0x8002ab0c;break;} // 987f0004 stb 3, 4(31)
case 0x8002ab0c:{if(!word((r[29]+uint32_t(88)),r[3]))return {};c.pc=0x8002ab10;break;} // 807d0058 lwz 3, 88(29)
case 0x8002ab10:{if(!byte((r[3]+uint32_t(2)),r[0]))return {};c.pc=0x8002ab14;break;} // 88030002 lbz 0, 2(3)
case 0x8002ab14:{r[0]=rotate(r[0],0)&0x00000002u;compare(c,0,signedWord(r[0]),0);c.pc=0x8002ab18;break;} // 540007bd rlwinm. 0, 0, 0, 30, 30
case 0x8002ab18:{c.pc=c.condition[2]?0x8002ab98:0x8002ab1c;break;} // 41820080 bt 2, 0x8002ab98
case 0x8002ab1c:{if(!word((r[31]+uint32_t(0)),r[0]))return {};c.pc=0x8002ab20;break;} // 801f0000 lwz 0, 0(31)
case 0x8002ab20:{r[0]=r[0]|(uint32_t(16));c.pc=0x8002ab24;break;} // 60000010 ori 0, 0, 16
case 0x8002ab24:{if(!store((r[31]+uint32_t(0)),r[0]))return {};c.pc=0x8002ab28;break;} // 901f0000 stw 0, 0(31)
case 0x8002ab28:{if(!word((r[29]+uint32_t(88)),r[3]))return {};c.pc=0x8002ab2c;break;} // 807d0058 lwz 3, 88(29)
case 0x8002ab2c:{if(!byte((r[3]+uint32_t(2)),r[0]))return {};c.pc=0x8002ab30;break;} // 88030002 lbz 0, 2(3)
case 0x8002ab30:{r[0]=rotate(r[0],0)&0x00000010u;compare(c,0,signedWord(r[0]),0);c.pc=0x8002ab34;break;} // 540006f7 rlwinm. 0, 0, 0, 27, 27
case 0x8002ab34:{c.pc=c.condition[2]?0x8002ab44:0x8002ab38;break;} // 41820010 bt 2, 0x8002ab44
case 0x8002ab38:{if(!word((r[31]+uint32_t(0)),r[0]))return {};c.pc=0x8002ab3c;break;} // 801f0000 lwz 0, 0(31)
case 0x8002ab3c:{r[0]=r[0]|(uint32_t(32768));c.pc=0x8002ab40;break;} // 60008000 ori 0, 0, 32768
case 0x8002ab40:{if(!store((r[31]+uint32_t(0)),r[0]))return {};c.pc=0x8002ab44;break;} // 901f0000 stw 0, 0(31)
case 0x8002ab44:{if(!word((r[29]+uint32_t(88)),r[3]))return {};c.pc=0x8002ab48;break;} // 807d0058 lwz 3, 88(29)
case 0x8002ab48:{if(!byte((r[3]+uint32_t(0)),r[0]))return {};c.pc=0x8002ab4c;break;} // 88030000 lbz 0, 0(3)
case 0x8002ab4c:{compare(c,0,signedWord(r[0]),int64_t(2));c.pc=0x8002ab50;break;} // 2c000002 cmpwi 0, 2
case 0x8002ab50:{c.pc=c.condition[2]?0x8002ab84:0x8002ab54;break;} // 41820034 bt 2, 0x8002ab84
case 0x8002ab54:{c.pc=!c.condition[0]?0x8002ab90:0x8002ab58;break;} // 4080003c bf 0, 0x8002ab90
case 0x8002ab58:{compare(c,0,signedWord(r[0]),int64_t(0));c.pc=0x8002ab5c;break;} // 2c000000 cmpwi 0, 0
case 0x8002ab5c:{c.pc=c.condition[2]?0x8002ab6c:0x8002ab60;break;} // 41820010 bt 2, 0x8002ab6c
case 0x8002ab60:{c.pc=!c.condition[0]?0x8002ab78:0x8002ab64;break;} // 40800018 bf 0, 0x8002ab78
case 0x8002ab64:{c.pc=0x8002ab90;break;} // 4800002c b 0x8002ab90
case 0x8002ab68:{c.pc=0x8002ab90;break;} // 48000028 b 0x8002ab90
case 0x8002ab6c:{r[0]=uint32_t(2);c.pc=0x8002ab70;break;} // 38000002 li 0, 2
case 0x8002ab70:{if(!s.storeByte || !s.storeByte(s.user,(r[31]+uint32_t(4)),uint8_t(r[0])))return {};c.pc=0x8002ab74;break;} // 981f0004 stb 0, 4(31)
case 0x8002ab74:{c.pc=0x8002ab98;break;} // 48000024 b 0x8002ab98
case 0x8002ab78:{r[0]=uint32_t(0);c.pc=0x8002ab7c;break;} // 38000000 li 0, 0
case 0x8002ab7c:{if(!s.storeByte || !s.storeByte(s.user,(r[31]+uint32_t(4)),uint8_t(r[0])))return {};c.pc=0x8002ab80;break;} // 981f0004 stb 0, 4(31)
case 0x8002ab80:{c.pc=0x8002ab98;break;} // 48000018 b 0x8002ab98
case 0x8002ab84:{r[0]=uint32_t(1);c.pc=0x8002ab88;break;} // 38000001 li 0, 1
case 0x8002ab88:{if(!s.storeByte || !s.storeByte(s.user,(r[31]+uint32_t(4)),uint8_t(r[0])))return {};c.pc=0x8002ab8c;break;} // 981f0004 stb 0, 4(31)
case 0x8002ab8c:{c.pc=0x8002ab98;break;} // 4800000c b 0x8002ab98
case 0x8002ab90:{r[0]=uint32_t(2);c.pc=0x8002ab94;break;} // 38000002 li 0, 2
case 0x8002ab94:{if(!s.storeByte || !s.storeByte(s.user,(r[31]+uint32_t(4)),uint8_t(r[0])))return {};c.pc=0x8002ab98;break;} // 981f0004 stb 0, 4(31)
case 0x8002ab98:{if(!word((r[31]+uint32_t(0)),r[0]))return {};c.pc=0x8002ab9c;break;} // 801f0000 lwz 0, 0(31)
case 0x8002ab9c:{r[0]=rotate(r[0],0)&0x00000010u;compare(c,0,signedWord(r[0]),0);c.pc=0x8002aba0;break;} // 540006f7 rlwinm. 0, 0, 0, 27, 27
case 0x8002aba0:{c.pc=c.condition[2]?0x8002abd0:0x8002aba4;break;} // 41820030 bt 2, 0x8002abd0
case 0x8002aba4:{compare(c,0,int64_t(r[30]),int64_t(0));c.pc=0x8002aba8;break;} // 281e0000 cmplwi 30, 0
case 0x8002aba8:{c.pc=c.condition[2]?0x8002ac10:0x8002abac;break;} // 41820068 bt 2, 0x8002ac10
case 0x8002abac:{if(!word((r[30]+uint32_t(0)),r[3]))return {};c.pc=0x8002abb0;break;} // 807e0000 lwz 3, 0(30)
case 0x8002abb0:{r[0]=rotate(r[3],0)&0x00000010u;compare(c,0,signedWord(r[0]),0);c.pc=0x8002abb4;break;} // 546006f7 rlwinm. 0, 3, 0, 27, 27
case 0x8002abb4:{c.pc=!c.condition[2]?0x8002abc0:0x8002abb8;break;} // 4082000c bf 2, 0x8002abc0
case 0x8002abb8:{r[0]=rotate(r[3],0)&0x00000020u;compare(c,0,signedWord(r[0]),0);c.pc=0x8002abbc;break;} // 546006b5 rlwinm. 0, 3, 0, 26, 26
case 0x8002abbc:{c.pc=c.condition[2]?0x8002ac10:0x8002abc0;break;} // 41820054 bt 2, 0x8002ac10
case 0x8002abc0:{if(!word((r[31]+uint32_t(0)),r[0]))return {};c.pc=0x8002abc4;break;} // 801f0000 lwz 0, 0(31)
case 0x8002abc4:{r[0]=rotate(r[0],0)&0xffffffefu;c.pc=0x8002abc8;break;} // 54000734 rlwinm 0, 0, 0, 28, 26
case 0x8002abc8:{if(!store((r[31]+uint32_t(0)),r[0]))return {};c.pc=0x8002abcc;break;} // 901f0000 stw 0, 0(31)
case 0x8002abcc:{c.pc=0x8002ac10;break;} // 48000044 b 0x8002ac10
case 0x8002abd0:{compare(c,0,int64_t(r[30]),int64_t(0));c.pc=0x8002abd4;break;} // 281e0000 cmplwi 30, 0
case 0x8002abd4:{c.pc=c.condition[2]?0x8002ac10:0x8002abd8;break;} // 4182003c bt 2, 0x8002ac10
case 0x8002abd8:{if(!word((r[30]+uint32_t(0)),r[3]))return {};c.pc=0x8002abdc;break;} // 807e0000 lwz 3, 0(30)
case 0x8002abdc:{r[0]=rotate(r[3],0)&0x00000010u;compare(c,0,signedWord(r[0]),0);c.pc=0x8002abe0;break;} // 546006f7 rlwinm. 0, 3, 0, 27, 27
case 0x8002abe0:{c.pc=!c.condition[2]?0x8002abec:0x8002abe4;break;} // 4082000c bf 2, 0x8002abec
case 0x8002abe4:{r[0]=rotate(r[3],0)&0x00000020u;compare(c,0,signedWord(r[0]),0);c.pc=0x8002abe8;break;} // 546006b5 rlwinm. 0, 3, 0, 26, 26
case 0x8002abe8:{c.pc=c.condition[2]?0x8002ac10:0x8002abec;break;} // 41820028 bt 2, 0x8002ac10
case 0x8002abec:{if(!word((r[31]+uint32_t(0)),r[0]))return {};c.pc=0x8002abf0;break;} // 801f0000 lwz 0, 0(31)
case 0x8002abf0:{r[0]=r[0]|(uint32_t(32));c.pc=0x8002abf4;break;} // 60000020 ori 0, 0, 32
case 0x8002abf4:{if(!store((r[31]+uint32_t(0)),r[0]))return {};c.pc=0x8002abf8;break;} // 901f0000 stw 0, 0(31)
case 0x8002abf8:{if(!word((r[30]+uint32_t(0)),r[0]))return {};c.pc=0x8002abfc;break;} // 801e0000 lwz 0, 0(30)
case 0x8002abfc:{r[0]=rotate(r[0],0)&0x00008000u;compare(c,0,signedWord(r[0]),0);c.pc=0x8002ac00;break;} // 54000421 rlwinm. 0, 0, 0, 16, 16
case 0x8002ac00:{c.pc=c.condition[2]?0x8002ac10:0x8002ac04;break;} // 41820010 bt 2, 0x8002ac10
case 0x8002ac04:{if(!word((r[31]+uint32_t(0)),r[0]))return {};c.pc=0x8002ac08;break;} // 801f0000 lwz 0, 0(31)
case 0x8002ac08:{r[0]=r[0]|(uint32_t(32768));c.pc=0x8002ac0c;break;} // 60008000 ori 0, 0, 32768
case 0x8002ac0c:{if(!store((r[31]+uint32_t(0)),r[0]))return {};c.pc=0x8002ac10;break;} // 901f0000 stw 0, 0(31)
case 0x8002ac10:{if(!word((r[29]+uint32_t(88)),r[3]))return {};c.pc=0x8002ac14;break;} // 807d0058 lwz 3, 88(29)
case 0x8002ac14:{if(!byte((r[3]+uint32_t(2)),r[0]))return {};c.pc=0x8002ac18;break;} // 88030002 lbz 0, 2(3)
case 0x8002ac18:{r[0]=rotate(r[0],0)&0x00000004u;compare(c,0,signedWord(r[0]),0);c.pc=0x8002ac1c;break;} // 5400077b rlwinm. 0, 0, 0, 29, 29
case 0x8002ac1c:{c.pc=c.condition[2]?0x8002ac4c:0x8002ac20;break;} // 41820030 bt 2, 0x8002ac4c
case 0x8002ac20:{if(!word((r[31]+uint32_t(0)),r[0]))return {};c.pc=0x8002ac24;break;} // 801f0000 lwz 0, 0(31)
case 0x8002ac24:{r[0]=r[0]|(uint32_t(4));c.pc=0x8002ac28;break;} // 60000004 ori 0, 0, 4
case 0x8002ac28:{if(!store((r[31]+uint32_t(0)),r[0]))return {};c.pc=0x8002ac2c;break;} // 901f0000 stw 0, 0(31)
case 0x8002ac2c:{c.pc=0x8002ac4c;break;} // 48000020 b 0x8002ac4c
case 0x8002ac30:{r[3]=uint32_t(-32724)<<16;c.pc=0x8002ac34;break;} // 3c60802c lis 3, -32724
case 0x8002ac34:{r[5]=uint32_t(-32724)<<16;c.pc=0x8002ac38;break;} // 3ca0802c lis 5, -32724
case 0x8002ac38:{r[3]=r[3]+uint32_t(-816);c.pc=0x8002ac3c;break;} // 3863fcd0 addi 3, 3, -816
case 0x8002ac3c:{r[4]=uint32_t(741);c.pc=0x8002ac40;break;} // 388002e5 li 4, 741
case 0x8002ac40:{r[5]=r[5]+uint32_t(-804);c.pc=0x8002ac44;break;} // 38a5fcdc addi 5, 5, -804
case 0x8002ac44:{c.condition[6]=false;c.pc=0x8002ac48;break;} // 4cc63182 crclr 6
case 0x8002ac48:{if(!external(0x80297894,0x8002ac4c))return {};break;} // 4826cc4d bl 0x80297894
case 0x8002ac4c:{if(!word((r[29]+uint32_t(92)),r[0]))return {};c.pc=0x8002ac50;break;} // 801d005c lwz 0, 92(29)
case 0x8002ac50:{compare(c,0,signedWord(r[0]),int64_t(0));c.pc=0x8002ac54;break;} // 2c000000 cmpwi 0, 0
case 0x8002ac54:{c.pc=c.condition[2]?0x8002ac7c:0x8002ac58;break;} // 41820028 bt 2, 0x8002ac7c
case 0x8002ac58:{r[3]=r[29]+uint32_t(60);c.pc=0x8002ac5c;break;} // 387d003c addi 3, 29, 60
case 0x8002ac5c:{r[4]=r[29]+uint32_t(72);c.pc=0x8002ac60;break;} // 389d0048 addi 4, 29, 72
case 0x8002ac60:{r[5]=r[31]+uint32_t(16);c.pc=0x8002ac64;break;} // 38bf0010 addi 5, 31, 16
case 0x8002ac64:{if(!external(0x8029eb80,0x8002ac68))return {};break;} // 48273f1d bl 0x8029eb80
case 0x8002ac68:{r[3]=r[31]+uint32_t(16);c.pc=0x8002ac6c;break;} // 387f0010 addi 3, 31, 16
case 0x8002ac6c:{if(!word((r[2]+uint32_t(-31500)),f[1]))return {};c.pc=0x8002ac70;break;} // c02284f4 lfs 1, -31500(2)
case 0x8002ac70:{r[4]=r[3];c.pc=0x8002ac74;break;} // 7c641b78 mr 4, 3
case 0x8002ac74:{if(!external(0x8029ebc8,0x8002ac78))return {};break;} // 48273f55 bl 0x8029ebc8
case 0x8002ac78:{c.pc=0x8002ac94;break;} // 4800001c b 0x8002ac94
case 0x8002ac7c:{if(!word((r[29]+uint32_t(48)),r[3]))return {};c.pc=0x8002ac80;break;} // 807d0030 lwz 3, 48(29)
case 0x8002ac80:{if(!word((r[29]+uint32_t(52)),r[0]))return {};c.pc=0x8002ac84;break;} // 801d0034 lwz 0, 52(29)
case 0x8002ac84:{if(!store((r[31]+uint32_t(16)),r[3]))return {};c.pc=0x8002ac88;break;} // 907f0010 stw 3, 16(31)
case 0x8002ac88:{if(!store((r[31]+uint32_t(20)),r[0]))return {};c.pc=0x8002ac8c;break;} // 901f0014 stw 0, 20(31)
case 0x8002ac8c:{if(!word((r[29]+uint32_t(56)),r[0]))return {};c.pc=0x8002ac90;break;} // 801d0038 lwz 0, 56(29)
case 0x8002ac90:{if(!store((r[31]+uint32_t(24)),r[0]))return {};c.pc=0x8002ac94;break;} // 901f0018 stw 0, 24(31)
case 0x8002ac94:{if(!word((r[29]+uint32_t(92)),r[0]))return {};c.pc=0x8002ac98;break;} // 801d005c lwz 0, 92(29)
case 0x8002ac98:{compare(c,0,signedWord(r[0]),int64_t(0));c.pc=0x8002ac9c;break;} // 2c000000 cmpwi 0, 0
case 0x8002ac9c:{c.pc=c.condition[2]?0x8002ace4:0x8002aca0;break;} // 41820048 bt 2, 0x8002ace4
case 0x8002aca0:{r[3]=r[31]+uint32_t(28);c.pc=0x8002aca4;break;} // 387f001c addi 3, 31, 28
case 0x8002aca4:{r[4]=r[31]+uint32_t(16);c.pc=0x8002aca8;break;} // 389f0010 addi 4, 31, 16
case 0x8002aca8:{r[5]=r[1]+uint32_t(20);c.pc=0x8002acac;break;} // 38a10014 addi 5, 1, 20
case 0x8002acac:{if(!external(0x8029e6a4,0x8002acb0))return {};break;} // 482739f9 bl 0x8029e6a4
case 0x8002acb0:{r[3]=r[31]+uint32_t(28);c.pc=0x8002acb4;break;} // 387f001c addi 3, 31, 28
case 0x8002acb4:{r[4]=r[29]+uint32_t(72);c.pc=0x8002acb8;break;} // 389d0048 addi 4, 29, 72
case 0x8002acb8:{r[5]=r[1]+uint32_t(8);c.pc=0x8002acbc;break;} // 38a10008 addi 5, 1, 8
case 0x8002acbc:{if(!external(0x8029e6a4,0x8002acc0))return {};break;} // 482739e9 bl 0x8029e6a4
case 0x8002acc0:{r[3]=r[1]+uint32_t(20);c.pc=0x8002acc4;break;} // 38610014 addi 3, 1, 20
case 0x8002acc4:{r[4]=r[1]+uint32_t(8);c.pc=0x8002acc8;break;} // 38810008 addi 4, 1, 8
case 0x8002acc8:{if(!external(0x8029ee8c,0x8002accc))return {};break;} // 482741c5 bl 0x8029ee8c
case 0x8002accc:{if(!word((r[2]+uint32_t(-31368)),f[0]))return {};c.pc=0x8002acd0;break;} // c0028578 lfs 0, -31368(2)
case 0x8002acd0:{const float x=scalar(f[1]),y=scalar(f[0]);const unsigned field=0*4;c.condition[field]=x<y;c.condition[field+1]=x>y;c.condition[field+2]=x==y;c.condition[field+3]=std::isnan(x)||std::isnan(y);c.pc=0x8002acd4;break;} // fc010040 fcmpo 0, 1, 0
case 0x8002acd4:{c.pc=!c.condition[0]?0x8002ace4:0x8002acd8;break;} // 40800010 bf 0, 0x8002ace4
case 0x8002acd8:{if(!word((r[31]+uint32_t(0)),r[0]))return {};c.pc=0x8002acdc;break;} // 801f0000 lwz 0, 0(31)
case 0x8002acdc:{r[0]=r[0]|(uint32_t(2));c.pc=0x8002ace0;break;} // 60000002 ori 0, 0, 2
case 0x8002ace0:{if(!store((r[31]+uint32_t(0)),r[0]))return {};c.pc=0x8002ace4;break;} // 901f0000 stw 0, 0(31)
case 0x8002ace4:{if(!word((r[1]+uint32_t(292)),r[0]))return {};c.pc=0x8002ace8;break;} // 80010124 lwz 0, 292(1)
case 0x8002ace8:{r[3]=r[31];c.pc=0x8002acec;break;} // 7fe3fb78 mr 3, 31
case 0x8002acec:{if(!word((r[1]+uint32_t(284)),r[31]))return {};c.pc=0x8002acf0;break;} // 83e1011c lwz 31, 284(1)
case 0x8002acf0:{if(!word((r[1]+uint32_t(280)),r[30]))return {};c.pc=0x8002acf4;break;} // 83c10118 lwz 30, 280(1)
case 0x8002acf4:{if(!word((r[1]+uint32_t(276)),r[29]))return {};c.pc=0x8002acf8;break;} // 83a10114 lwz 29, 276(1)
case 0x8002acf8:{if(!word((r[1]+uint32_t(272)),r[28]))return {};c.pc=0x8002acfc;break;} // 83810110 lwz 28, 272(1)
case 0x8002acfc:{c.lr=r[0];c.pc=0x8002ad00;break;} // 7c0803a6 mtlr 0
case 0x8002ad00:{r[1]=r[1]+uint32_t(288);c.pc=0x8002ad04;break;} // 38210120 addi 1, 1, 288
case 0x8002ad04:{c.pc=c.lr;break;} // 4e800020 blr 
case 0x80028130:{const auto target=(r[1]+uint32_t(-32));if(!store(target,r[1]))return {};r[1]=target;c.pc=0x80028134;break;} // 9421ffe0 stwu 1, -32(1)
case 0x80028134:{r[0]=c.lr;c.pc=0x80028138;break;} // 7c0802a6 mflr 0
case 0x80028138:{if(!store((r[1]+uint32_t(36)),r[0]))return {};c.pc=0x8002813c;break;} // 90010024 stw 0, 36(1)
case 0x8002813c:{if(!store((r[1]+uint32_t(28)),r[31]))return {};c.pc=0x80028140;break;} // 93e1001c stw 31, 28(1)
case 0x80028140:{r[31]=r[5];c.pc=0x80028144;break;} // 7cbf2b78 mr 31, 5
case 0x80028144:{if(!store((r[1]+uint32_t(24)),r[30]))return {};c.pc=0x80028148;break;} // 93c10018 stw 30, 24(1)
case 0x80028148:{r[30]=r[4];c.pc=0x8002814c;break;} // 7c9e2378 mr 30, 4
case 0x8002814c:{r[4]=uint32_t(0);c.pc=0x80028150;break;} // 38800000 li 4, 0
case 0x80028150:{if(!store((r[1]+uint32_t(20)),r[29]))return {};c.pc=0x80028154;break;} // 93a10014 stw 29, 20(1)
case 0x80028154:{r[29]=r[3];c.pc=0x80028158;break;} // 7c7d1b78 mr 29, 3
case 0x80028158:{if(!store((r[1]+uint32_t(16)),r[28]))return {};c.pc=0x8002815c;break;} // 93810010 stw 28, 16(1)
case 0x8002815c:{if(!word((r[3]+uint32_t(0)),r[3]))return {};c.pc=0x80028160;break;} // 80630000 lwz 3, 0(3)
case 0x80028160:{r[0]=rotate(r[3],0)&0x00000200u;compare(c,0,signedWord(r[0]),0);c.pc=0x80028164;break;} // 546005ad rlwinm. 0, 3, 0, 22, 22
case 0x80028164:{c.pc=c.condition[2]?0x8002816c:0x80028168;break;} // 41820008 bt 2, 0x8002816c
case 0x80028168:{r[4]=uint32_t(1);c.pc=0x8002816c;break;} // 38800001 li 4, 1
case 0x8002816c:{compare(c,0,signedWord(r[31]),int64_t(0));c.pc=0x80028170;break;} // 2c1f0000 cmpwi 31, 0
case 0x80028170:{c.pc=c.condition[2]?0x80028178:0x80028174;break;} // 41820008 bt 2, 0x80028178
case 0x80028174:{r[4]=uint32_t(1);c.pc=0x80028178;break;} // 38800001 li 4, 1
case 0x80028178:{compare(c,0,signedWord(r[4]),int64_t(0));c.pc=0x8002817c;break;} // 2c040000 cmpwi 4, 0
case 0x8002817c:{c.pc=c.condition[2]?0x80028358:0x80028180;break;} // 418201dc bt 2, 0x80028358
case 0x80028180:{r[0]=rotate(r[3],0)&0x00002000u;compare(c,0,signedWord(r[0]),0);c.pc=0x80028184;break;} // 546004a5 rlwinm. 0, 3, 0, 18, 18
case 0x80028184:{c.pc=c.condition[2]?0x8002819c:0x80028188;break;} // 41820018 bt 2, 0x8002819c
case 0x80028188:{r[3]=r[30];c.pc=0x8002818c;break;} // 7fc3f378 mr 3, 30
case 0x8002818c:{r[4]=r[29]+uint32_t(124);c.pc=0x80028190;break;} // 389d007c addi 4, 29, 124
case 0x80028190:{r[5]=r[29]+uint32_t(28);c.pc=0x80028194;break;} // 38bd001c addi 5, 29, 28
case 0x80028194:{if(!external(0x8029dd64,0x80028198))return {};break;} // 48275bd1 bl 0x8029dd64
case 0x80028198:{c.pc=0x800281ac;break;} // 48000014 b 0x800281ac
case 0x8002819c:{r[3]=r[30];c.pc=0x800281a0;break;} // 7fc3f378 mr 3, 30
case 0x800281a0:{r[4]=r[29]+uint32_t(76);c.pc=0x800281a4;break;} // 389d004c addi 4, 29, 76
case 0x800281a4:{r[5]=r[29]+uint32_t(28);c.pc=0x800281a8;break;} // 38bd001c addi 5, 29, 28
case 0x800281a8:{if(!external(0x8029dd64,0x800281ac))return {};break;} // 48275bbd bl 0x8029dd64
case 0x800281ac:{if(!word((r[29]+uint32_t(0)),r[0]))return {};c.pc=0x800281b0;break;} // 801d0000 lwz 0, 0(29)
case 0x800281b0:{r[0]=rotate(r[0],0)&0x00200000u;compare(c,0,signedWord(r[0]),0);c.pc=0x800281b4;break;} // 54000295 rlwinm. 0, 0, 0, 10, 10
case 0x800281b4:{c.pc=c.condition[2]?0x800281c8:0x800281b8;break;} // 41820014 bt 2, 0x800281c8
case 0x800281b8:{r[3]=r[29]+uint32_t(28);c.pc=0x800281bc;break;} // 387d001c addi 3, 29, 28
case 0x800281bc:{r[4]=r[29]+uint32_t(172);c.pc=0x800281c0;break;} // 389d00ac addi 4, 29, 172
case 0x800281c0:{r[5]=r[3];c.pc=0x800281c4;break;} // 7c651b78 mr 5, 3
case 0x800281c4:{if(!external(0x8029dd64,0x800281c8))return {};break;} // 48275ba1 bl 0x8029dd64
case 0x800281c8:{if(!word((r[29]+uint32_t(0)),r[0]))return {};c.pc=0x800281cc;break;} // 801d0000 lwz 0, 0(29)
case 0x800281cc:{r[0]=rotate(r[0],0)&0xfffffdffu;c.pc=0x800281d0;break;} // 540005ea rlwinm 0, 0, 0, 23, 21
case 0x800281d0:{if(!store((r[29]+uint32_t(0)),r[0]))return {};c.pc=0x800281d4;break;} // 901d0000 stw 0, 0(29)
case 0x800281d4:{if(!word((r[29]+uint32_t(228)),r[28]))return {};c.pc=0x800281d8;break;} // 839d00e4 lwz 28, 228(29)
case 0x800281d8:{compare(c,0,int64_t(r[28]),int64_t(0));c.pc=0x800281dc;break;} // 281c0000 cmplwi 28, 0
case 0x800281dc:{c.pc=c.condition[2]?0x80028268:0x800281e0;break;} // 4182008c bt 2, 0x80028268
case 0x800281e0:{if(!word((r[28]+uint32_t(0)),r[0]))return {};c.pc=0x800281e4;break;} // 801c0000 lwz 0, 0(28)
case 0x800281e4:{r[0]=rotate(r[0],0)&0x00002000u;compare(c,0,signedWord(r[0]),0);c.pc=0x800281e8;break;} // 540004a5 rlwinm. 0, 0, 0, 18, 18
case 0x800281e8:{c.pc=c.condition[2]?0x80028200:0x800281ec;break;} // 41820018 bt 2, 0x80028200
case 0x800281ec:{r[3]=r[29]+uint32_t(28);c.pc=0x800281f0;break;} // 387d001c addi 3, 29, 28
case 0x800281f0:{r[4]=r[28]+uint32_t(124);c.pc=0x800281f4;break;} // 389c007c addi 4, 28, 124
case 0x800281f4:{r[5]=r[28]+uint32_t(28);c.pc=0x800281f8;break;} // 38bc001c addi 5, 28, 28
case 0x800281f8:{if(!external(0x8029dd64,0x800281fc))return {};break;} // 48275b6d bl 0x8029dd64
case 0x800281fc:{c.pc=0x80028210;break;} // 48000014 b 0x80028210
case 0x80028200:{r[3]=r[29]+uint32_t(28);c.pc=0x80028204;break;} // 387d001c addi 3, 29, 28
case 0x80028204:{r[4]=r[28]+uint32_t(76);c.pc=0x80028208;break;} // 389c004c addi 4, 28, 76
case 0x80028208:{r[5]=r[28]+uint32_t(28);c.pc=0x8002820c;break;} // 38bc001c addi 5, 28, 28
case 0x8002820c:{if(!external(0x8029dd64,0x80028210))return {};break;} // 48275b59 bl 0x8029dd64
case 0x80028210:{if(!word((r[28]+uint32_t(0)),r[0]))return {};c.pc=0x80028214;break;} // 801c0000 lwz 0, 0(28)
case 0x80028214:{r[0]=rotate(r[0],0)&0x00200000u;compare(c,0,signedWord(r[0]),0);c.pc=0x80028218;break;} // 54000295 rlwinm. 0, 0, 0, 10, 10
case 0x80028218:{c.pc=c.condition[2]?0x8002822c:0x8002821c;break;} // 41820014 bt 2, 0x8002822c
case 0x8002821c:{r[3]=r[28]+uint32_t(28);c.pc=0x80028220;break;} // 387c001c addi 3, 28, 28
case 0x80028220:{r[4]=r[28]+uint32_t(172);c.pc=0x80028224;break;} // 389c00ac addi 4, 28, 172
case 0x80028224:{r[5]=r[3];c.pc=0x80028228;break;} // 7c651b78 mr 5, 3
case 0x80028228:{if(!external(0x8029dd64,0x8002822c))return {};break;} // 48275b3d bl 0x8029dd64
case 0x8002822c:{if(!word((r[28]+uint32_t(0)),r[0]))return {};c.pc=0x80028230;break;} // 801c0000 lwz 0, 0(28)
case 0x80028230:{r[0]=rotate(r[0],0)&0xfffffdffu;c.pc=0x80028234;break;} // 540005ea rlwinm 0, 0, 0, 23, 21
case 0x80028234:{if(!store((r[28]+uint32_t(0)),r[0]))return {};c.pc=0x80028238;break;} // 901c0000 stw 0, 0(28)
case 0x80028238:{if(!word((r[28]+uint32_t(228)),r[3]))return {};c.pc=0x8002823c;break;} // 807c00e4 lwz 3, 228(28)
case 0x8002823c:{compare(c,0,int64_t(r[3]),int64_t(0));c.pc=0x80028240;break;} // 28030000 cmplwi 3, 0
case 0x80028240:{c.pc=c.condition[2]?0x80028250:0x80028244;break;} // 41820010 bt 2, 0x80028250
case 0x80028244:{r[4]=r[28]+uint32_t(28);c.pc=0x80028248;break;} // 389c001c addi 4, 28, 28
case 0x80028248:{r[5]=uint32_t(1);c.pc=0x8002824c;break;} // 38a00001 li 5, 1
case 0x8002824c:{c.lr=0x80028250;c.pc=0x80028130;break;} // 4bfffee5 bl 0x80028130
case 0x80028250:{if(!word((r[28]+uint32_t(232)),r[3]))return {};c.pc=0x80028254;break;} // 807c00e8 lwz 3, 232(28)
case 0x80028254:{compare(c,0,int64_t(r[3]),int64_t(0));c.pc=0x80028258;break;} // 28030000 cmplwi 3, 0
case 0x80028258:{c.pc=c.condition[2]?0x80028268:0x8002825c;break;} // 41820010 bt 2, 0x80028268
case 0x8002825c:{r[4]=r[29]+uint32_t(28);c.pc=0x80028260;break;} // 389d001c addi 4, 29, 28
case 0x80028260:{r[5]=uint32_t(1);c.pc=0x80028264;break;} // 38a00001 li 5, 1
case 0x80028264:{c.lr=0x80028268;c.pc=0x80028130;break;} // 4bfffecd bl 0x80028130
case 0x80028268:{if(!word((r[29]+uint32_t(232)),r[28]))return {};c.pc=0x8002826c;break;} // 839d00e8 lwz 28, 232(29)
case 0x8002826c:{compare(c,0,int64_t(r[28]),int64_t(0));c.pc=0x80028270;break;} // 281c0000 cmplwi 28, 0
case 0x80028270:{c.pc=c.condition[2]?0x80028530:0x80028274;break;} // 418202c0 bt 2, 0x80028530
case 0x80028274:{if(!word((r[28]+uint32_t(0)),r[4]))return {};c.pc=0x80028278;break;} // 809c0000 lwz 4, 0(28)
case 0x80028278:{r[3]=uint32_t(0);c.pc=0x8002827c;break;} // 38600000 li 3, 0
case 0x8002827c:{r[0]=rotate(r[4],0)&0x00000200u;compare(c,0,signedWord(r[0]),0);c.pc=0x80028280;break;} // 548005ad rlwinm. 0, 4, 0, 22, 22
case 0x80028280:{c.pc=c.condition[2]?0x80028288:0x80028284;break;} // 41820008 bt 2, 0x80028288
case 0x80028284:{r[3]=uint32_t(1);c.pc=0x80028288;break;} // 38600001 li 3, 1
case 0x80028288:{compare(c,0,signedWord(r[31]),int64_t(0));c.pc=0x8002828c;break;} // 2c1f0000 cmpwi 31, 0
case 0x8002828c:{c.pc=c.condition[2]?0x80028294:0x80028290;break;} // 41820008 bt 2, 0x80028294
case 0x80028290:{r[3]=uint32_t(1);c.pc=0x80028294;break;} // 38600001 li 3, 1
case 0x80028294:{compare(c,0,signedWord(r[3]),int64_t(0));c.pc=0x80028298;break;} // 2c030000 cmpwi 3, 0
case 0x80028298:{c.pc=c.condition[2]?0x80028324:0x8002829c;break;} // 4182008c bt 2, 0x80028324
case 0x8002829c:{r[0]=rotate(r[4],0)&0x00002000u;compare(c,0,signedWord(r[0]),0);c.pc=0x800282a0;break;} // 548004a5 rlwinm. 0, 4, 0, 18, 18
case 0x800282a0:{c.pc=c.condition[2]?0x800282b8:0x800282a4;break;} // 41820018 bt 2, 0x800282b8
case 0x800282a4:{r[3]=r[30];c.pc=0x800282a8;break;} // 7fc3f378 mr 3, 30
case 0x800282a8:{r[4]=r[28]+uint32_t(124);c.pc=0x800282ac;break;} // 389c007c addi 4, 28, 124
case 0x800282ac:{r[5]=r[28]+uint32_t(28);c.pc=0x800282b0;break;} // 38bc001c addi 5, 28, 28
case 0x800282b0:{if(!external(0x8029dd64,0x800282b4))return {};break;} // 48275ab5 bl 0x8029dd64
case 0x800282b4:{c.pc=0x800282c8;break;} // 48000014 b 0x800282c8
case 0x800282b8:{r[3]=r[30];c.pc=0x800282bc;break;} // 7fc3f378 mr 3, 30
case 0x800282bc:{r[4]=r[28]+uint32_t(76);c.pc=0x800282c0;break;} // 389c004c addi 4, 28, 76
case 0x800282c0:{r[5]=r[28]+uint32_t(28);c.pc=0x800282c4;break;} // 38bc001c addi 5, 28, 28
case 0x800282c4:{if(!external(0x8029dd64,0x800282c8))return {};break;} // 48275aa1 bl 0x8029dd64
case 0x800282c8:{if(!word((r[28]+uint32_t(0)),r[0]))return {};c.pc=0x800282cc;break;} // 801c0000 lwz 0, 0(28)
case 0x800282cc:{r[0]=rotate(r[0],0)&0x00200000u;compare(c,0,signedWord(r[0]),0);c.pc=0x800282d0;break;} // 54000295 rlwinm. 0, 0, 0, 10, 10
case 0x800282d0:{c.pc=c.condition[2]?0x800282e4:0x800282d4;break;} // 41820014 bt 2, 0x800282e4
case 0x800282d4:{r[3]=r[28]+uint32_t(28);c.pc=0x800282d8;break;} // 387c001c addi 3, 28, 28
case 0x800282d8:{r[4]=r[28]+uint32_t(172);c.pc=0x800282dc;break;} // 389c00ac addi 4, 28, 172
case 0x800282dc:{r[5]=r[3];c.pc=0x800282e0;break;} // 7c651b78 mr 5, 3
case 0x800282e0:{if(!external(0x8029dd64,0x800282e4))return {};break;} // 48275a85 bl 0x8029dd64
case 0x800282e4:{if(!word((r[28]+uint32_t(0)),r[0]))return {};c.pc=0x800282e8;break;} // 801c0000 lwz 0, 0(28)
case 0x800282e8:{r[0]=rotate(r[0],0)&0xfffffdffu;c.pc=0x800282ec;break;} // 540005ea rlwinm 0, 0, 0, 23, 21
case 0x800282ec:{if(!store((r[28]+uint32_t(0)),r[0]))return {};c.pc=0x800282f0;break;} // 901c0000 stw 0, 0(28)
case 0x800282f0:{if(!word((r[28]+uint32_t(228)),r[3]))return {};c.pc=0x800282f4;break;} // 807c00e4 lwz 3, 228(28)
case 0x800282f4:{compare(c,0,int64_t(r[3]),int64_t(0));c.pc=0x800282f8;break;} // 28030000 cmplwi 3, 0
case 0x800282f8:{c.pc=c.condition[2]?0x80028308:0x800282fc;break;} // 41820010 bt 2, 0x80028308
case 0x800282fc:{r[4]=r[28]+uint32_t(28);c.pc=0x80028300;break;} // 389c001c addi 4, 28, 28
case 0x80028300:{r[5]=uint32_t(1);c.pc=0x80028304;break;} // 38a00001 li 5, 1
case 0x80028304:{c.lr=0x80028308;c.pc=0x80028130;break;} // 4bfffe2d bl 0x80028130
case 0x80028308:{if(!word((r[28]+uint32_t(232)),r[3]))return {};c.pc=0x8002830c;break;} // 807c00e8 lwz 3, 232(28)
case 0x8002830c:{compare(c,0,int64_t(r[3]),int64_t(0));c.pc=0x80028310;break;} // 28030000 cmplwi 3, 0
case 0x80028310:{c.pc=c.condition[2]?0x80028530:0x80028314;break;} // 41820220 bt 2, 0x80028530
case 0x80028314:{r[4]=r[30];c.pc=0x80028318;break;} // 7fc4f378 mr 4, 30
case 0x80028318:{r[5]=r[31];c.pc=0x8002831c;break;} // 7fe5fb78 mr 5, 31
case 0x8002831c:{c.lr=0x80028320;c.pc=0x80028130;break;} // 4bfffe15 bl 0x80028130
case 0x80028320:{c.pc=0x80028530;break;} // 48000210 b 0x80028530
case 0x80028324:{if(!word((r[28]+uint32_t(228)),r[3]))return {};c.pc=0x80028328;break;} // 807c00e4 lwz 3, 228(28)
case 0x80028328:{compare(c,0,int64_t(r[3]),int64_t(0));c.pc=0x8002832c;break;} // 28030000 cmplwi 3, 0
case 0x8002832c:{c.pc=c.condition[2]?0x8002833c:0x80028330;break;} // 41820010 bt 2, 0x8002833c
case 0x80028330:{r[5]=r[31];c.pc=0x80028334;break;} // 7fe5fb78 mr 5, 31
case 0x80028334:{r[4]=r[28]+uint32_t(28);c.pc=0x80028338;break;} // 389c001c addi 4, 28, 28
case 0x80028338:{c.lr=0x8002833c;c.pc=0x80028130;break;} // 4bfffdf9 bl 0x80028130
case 0x8002833c:{if(!word((r[28]+uint32_t(232)),r[3]))return {};c.pc=0x80028340;break;} // 807c00e8 lwz 3, 232(28)
case 0x80028340:{compare(c,0,int64_t(r[3]),int64_t(0));c.pc=0x80028344;break;} // 28030000 cmplwi 3, 0
case 0x80028344:{c.pc=c.condition[2]?0x80028530:0x80028348;break;} // 418201ec bt 2, 0x80028530
case 0x80028348:{r[4]=r[30];c.pc=0x8002834c;break;} // 7fc4f378 mr 4, 30
case 0x8002834c:{r[5]=r[31];c.pc=0x80028350;break;} // 7fe5fb78 mr 5, 31
case 0x80028350:{c.lr=0x80028354;c.pc=0x80028130;break;} // 4bfffde1 bl 0x80028130
case 0x80028354:{c.pc=0x80028530;break;} // 480001dc b 0x80028530
case 0x80028358:{if(!word((r[29]+uint32_t(228)),r[28]))return {};c.pc=0x8002835c;break;} // 839d00e4 lwz 28, 228(29)
case 0x8002835c:{compare(c,0,int64_t(r[28]),int64_t(0));c.pc=0x80028360;break;} // 281c0000 cmplwi 28, 0
case 0x80028360:{c.pc=c.condition[2]?0x80028444:0x80028364;break;} // 418200e4 bt 2, 0x80028444
case 0x80028364:{if(!word((r[28]+uint32_t(0)),r[4]))return {};c.pc=0x80028368;break;} // 809c0000 lwz 4, 0(28)
case 0x80028368:{r[3]=uint32_t(0);c.pc=0x8002836c;break;} // 38600000 li 3, 0
case 0x8002836c:{r[0]=rotate(r[4],0)&0x00000200u;compare(c,0,signedWord(r[0]),0);c.pc=0x80028370;break;} // 548005ad rlwinm. 0, 4, 0, 22, 22
case 0x80028370:{c.pc=c.condition[2]?0x80028378:0x80028374;break;} // 41820008 bt 2, 0x80028378
case 0x80028374:{r[3]=uint32_t(1);c.pc=0x80028378;break;} // 38600001 li 3, 1
case 0x80028378:{compare(c,0,signedWord(r[31]),int64_t(0));c.pc=0x8002837c;break;} // 2c1f0000 cmpwi 31, 0
case 0x8002837c:{c.pc=c.condition[2]?0x80028384:0x80028380;break;} // 41820008 bt 2, 0x80028384
case 0x80028380:{r[3]=uint32_t(1);c.pc=0x80028384;break;} // 38600001 li 3, 1
case 0x80028384:{compare(c,0,signedWord(r[3]),int64_t(0));c.pc=0x80028388;break;} // 2c030000 cmpwi 3, 0
case 0x80028388:{c.pc=c.condition[2]?0x80028414:0x8002838c;break;} // 4182008c bt 2, 0x80028414
case 0x8002838c:{r[0]=rotate(r[4],0)&0x00002000u;compare(c,0,signedWord(r[0]),0);c.pc=0x80028390;break;} // 548004a5 rlwinm. 0, 4, 0, 18, 18
case 0x80028390:{c.pc=c.condition[2]?0x800283a8:0x80028394;break;} // 41820018 bt 2, 0x800283a8
case 0x80028394:{r[3]=r[29]+uint32_t(28);c.pc=0x80028398;break;} // 387d001c addi 3, 29, 28
case 0x80028398:{r[4]=r[28]+uint32_t(124);c.pc=0x8002839c;break;} // 389c007c addi 4, 28, 124
case 0x8002839c:{r[5]=r[28]+uint32_t(28);c.pc=0x800283a0;break;} // 38bc001c addi 5, 28, 28
case 0x800283a0:{if(!external(0x8029dd64,0x800283a4))return {};break;} // 482759c5 bl 0x8029dd64
case 0x800283a4:{c.pc=0x800283b8;break;} // 48000014 b 0x800283b8
case 0x800283a8:{r[3]=r[29]+uint32_t(28);c.pc=0x800283ac;break;} // 387d001c addi 3, 29, 28
case 0x800283ac:{r[4]=r[28]+uint32_t(76);c.pc=0x800283b0;break;} // 389c004c addi 4, 28, 76
case 0x800283b0:{r[5]=r[28]+uint32_t(28);c.pc=0x800283b4;break;} // 38bc001c addi 5, 28, 28
case 0x800283b4:{if(!external(0x8029dd64,0x800283b8))return {};break;} // 482759b1 bl 0x8029dd64
case 0x800283b8:{if(!word((r[28]+uint32_t(0)),r[0]))return {};c.pc=0x800283bc;break;} // 801c0000 lwz 0, 0(28)
case 0x800283bc:{r[0]=rotate(r[0],0)&0x00200000u;compare(c,0,signedWord(r[0]),0);c.pc=0x800283c0;break;} // 54000295 rlwinm. 0, 0, 0, 10, 10
case 0x800283c0:{c.pc=c.condition[2]?0x800283d4:0x800283c4;break;} // 41820014 bt 2, 0x800283d4
case 0x800283c4:{r[3]=r[28]+uint32_t(28);c.pc=0x800283c8;break;} // 387c001c addi 3, 28, 28
case 0x800283c8:{r[4]=r[28]+uint32_t(172);c.pc=0x800283cc;break;} // 389c00ac addi 4, 28, 172
case 0x800283cc:{r[5]=r[3];c.pc=0x800283d0;break;} // 7c651b78 mr 5, 3
case 0x800283d0:{if(!external(0x8029dd64,0x800283d4))return {};break;} // 48275995 bl 0x8029dd64
case 0x800283d4:{if(!word((r[28]+uint32_t(0)),r[0]))return {};c.pc=0x800283d8;break;} // 801c0000 lwz 0, 0(28)
case 0x800283d8:{r[0]=rotate(r[0],0)&0xfffffdffu;c.pc=0x800283dc;break;} // 540005ea rlwinm 0, 0, 0, 23, 21
case 0x800283dc:{if(!store((r[28]+uint32_t(0)),r[0]))return {};c.pc=0x800283e0;break;} // 901c0000 stw 0, 0(28)
case 0x800283e0:{if(!word((r[28]+uint32_t(228)),r[3]))return {};c.pc=0x800283e4;break;} // 807c00e4 lwz 3, 228(28)
case 0x800283e4:{compare(c,0,int64_t(r[3]),int64_t(0));c.pc=0x800283e8;break;} // 28030000 cmplwi 3, 0
case 0x800283e8:{c.pc=c.condition[2]?0x800283f8:0x800283ec;break;} // 41820010 bt 2, 0x800283f8
case 0x800283ec:{r[4]=r[28]+uint32_t(28);c.pc=0x800283f0;break;} // 389c001c addi 4, 28, 28
case 0x800283f0:{r[5]=uint32_t(1);c.pc=0x800283f4;break;} // 38a00001 li 5, 1
case 0x800283f4:{c.lr=0x800283f8;c.pc=0x80028130;break;} // 4bfffd3d bl 0x80028130
case 0x800283f8:{if(!word((r[28]+uint32_t(232)),r[3]))return {};c.pc=0x800283fc;break;} // 807c00e8 lwz 3, 232(28)
case 0x800283fc:{compare(c,0,int64_t(r[3]),int64_t(0));c.pc=0x80028400;break;} // 28030000 cmplwi 3, 0
case 0x80028400:{c.pc=c.condition[2]?0x80028444:0x80028404;break;} // 41820044 bt 2, 0x80028444
case 0x80028404:{r[5]=r[31];c.pc=0x80028408;break;} // 7fe5fb78 mr 5, 31
case 0x80028408:{r[4]=r[29]+uint32_t(28);c.pc=0x8002840c;break;} // 389d001c addi 4, 29, 28
case 0x8002840c:{c.lr=0x80028410;c.pc=0x80028130;break;} // 4bfffd25 bl 0x80028130
case 0x80028410:{c.pc=0x80028444;break;} // 48000034 b 0x80028444
case 0x80028414:{if(!word((r[28]+uint32_t(228)),r[3]))return {};c.pc=0x80028418;break;} // 807c00e4 lwz 3, 228(28)
case 0x80028418:{compare(c,0,int64_t(r[3]),int64_t(0));c.pc=0x8002841c;break;} // 28030000 cmplwi 3, 0
case 0x8002841c:{c.pc=c.condition[2]?0x8002842c:0x80028420;break;} // 41820010 bt 2, 0x8002842c
case 0x80028420:{r[5]=r[31];c.pc=0x80028424;break;} // 7fe5fb78 mr 5, 31
case 0x80028424:{r[4]=r[28]+uint32_t(28);c.pc=0x80028428;break;} // 389c001c addi 4, 28, 28
case 0x80028428:{c.lr=0x8002842c;c.pc=0x80028130;break;} // 4bfffd09 bl 0x80028130
case 0x8002842c:{if(!word((r[28]+uint32_t(232)),r[3]))return {};c.pc=0x80028430;break;} // 807c00e8 lwz 3, 232(28)
case 0x80028430:{compare(c,0,int64_t(r[3]),int64_t(0));c.pc=0x80028434;break;} // 28030000 cmplwi 3, 0
case 0x80028434:{c.pc=c.condition[2]?0x80028444:0x80028438;break;} // 41820010 bt 2, 0x80028444
case 0x80028438:{r[5]=r[31];c.pc=0x8002843c;break;} // 7fe5fb78 mr 5, 31
case 0x8002843c:{r[4]=r[29]+uint32_t(28);c.pc=0x80028440;break;} // 389d001c addi 4, 29, 28
case 0x80028440:{c.lr=0x80028444;c.pc=0x80028130;break;} // 4bfffcf1 bl 0x80028130
case 0x80028444:{if(!word((r[29]+uint32_t(232)),r[28]))return {};c.pc=0x80028448;break;} // 839d00e8 lwz 28, 232(29)
case 0x80028448:{compare(c,0,int64_t(r[28]),int64_t(0));c.pc=0x8002844c;break;} // 281c0000 cmplwi 28, 0
case 0x8002844c:{c.pc=c.condition[2]?0x80028530:0x80028450;break;} // 418200e4 bt 2, 0x80028530
case 0x80028450:{if(!word((r[28]+uint32_t(0)),r[4]))return {};c.pc=0x80028454;break;} // 809c0000 lwz 4, 0(28)
case 0x80028454:{r[3]=uint32_t(0);c.pc=0x80028458;break;} // 38600000 li 3, 0
case 0x80028458:{r[0]=rotate(r[4],0)&0x00000200u;compare(c,0,signedWord(r[0]),0);c.pc=0x8002845c;break;} // 548005ad rlwinm. 0, 4, 0, 22, 22
case 0x8002845c:{c.pc=c.condition[2]?0x80028464:0x80028460;break;} // 41820008 bt 2, 0x80028464
case 0x80028460:{r[3]=uint32_t(1);c.pc=0x80028464;break;} // 38600001 li 3, 1
case 0x80028464:{compare(c,0,signedWord(r[31]),int64_t(0));c.pc=0x80028468;break;} // 2c1f0000 cmpwi 31, 0
case 0x80028468:{c.pc=c.condition[2]?0x80028470:0x8002846c;break;} // 41820008 bt 2, 0x80028470
case 0x8002846c:{r[3]=uint32_t(1);c.pc=0x80028470;break;} // 38600001 li 3, 1
case 0x80028470:{compare(c,0,signedWord(r[3]),int64_t(0));c.pc=0x80028474;break;} // 2c030000 cmpwi 3, 0
case 0x80028474:{c.pc=c.condition[2]?0x80028500:0x80028478;break;} // 4182008c bt 2, 0x80028500
case 0x80028478:{r[0]=rotate(r[4],0)&0x00002000u;compare(c,0,signedWord(r[0]),0);c.pc=0x8002847c;break;} // 548004a5 rlwinm. 0, 4, 0, 18, 18
case 0x8002847c:{c.pc=c.condition[2]?0x80028494:0x80028480;break;} // 41820018 bt 2, 0x80028494
case 0x80028480:{r[3]=r[30];c.pc=0x80028484;break;} // 7fc3f378 mr 3, 30
case 0x80028484:{r[4]=r[28]+uint32_t(124);c.pc=0x80028488;break;} // 389c007c addi 4, 28, 124
case 0x80028488:{r[5]=r[28]+uint32_t(28);c.pc=0x8002848c;break;} // 38bc001c addi 5, 28, 28
case 0x8002848c:{if(!external(0x8029dd64,0x80028490))return {};break;} // 482758d9 bl 0x8029dd64
case 0x80028490:{c.pc=0x800284a4;break;} // 48000014 b 0x800284a4
case 0x80028494:{r[3]=r[30];c.pc=0x80028498;break;} // 7fc3f378 mr 3, 30
case 0x80028498:{r[4]=r[28]+uint32_t(76);c.pc=0x8002849c;break;} // 389c004c addi 4, 28, 76
case 0x8002849c:{r[5]=r[28]+uint32_t(28);c.pc=0x800284a0;break;} // 38bc001c addi 5, 28, 28
case 0x800284a0:{if(!external(0x8029dd64,0x800284a4))return {};break;} // 482758c5 bl 0x8029dd64
case 0x800284a4:{if(!word((r[28]+uint32_t(0)),r[0]))return {};c.pc=0x800284a8;break;} // 801c0000 lwz 0, 0(28)
case 0x800284a8:{r[0]=rotate(r[0],0)&0x00200000u;compare(c,0,signedWord(r[0]),0);c.pc=0x800284ac;break;} // 54000295 rlwinm. 0, 0, 0, 10, 10
case 0x800284ac:{c.pc=c.condition[2]?0x800284c0:0x800284b0;break;} // 41820014 bt 2, 0x800284c0
case 0x800284b0:{r[3]=r[28]+uint32_t(28);c.pc=0x800284b4;break;} // 387c001c addi 3, 28, 28
case 0x800284b4:{r[4]=r[28]+uint32_t(172);c.pc=0x800284b8;break;} // 389c00ac addi 4, 28, 172
case 0x800284b8:{r[5]=r[3];c.pc=0x800284bc;break;} // 7c651b78 mr 5, 3
case 0x800284bc:{if(!external(0x8029dd64,0x800284c0))return {};break;} // 482758a9 bl 0x8029dd64
case 0x800284c0:{if(!word((r[28]+uint32_t(0)),r[0]))return {};c.pc=0x800284c4;break;} // 801c0000 lwz 0, 0(28)
case 0x800284c4:{r[0]=rotate(r[0],0)&0xfffffdffu;c.pc=0x800284c8;break;} // 540005ea rlwinm 0, 0, 0, 23, 21
case 0x800284c8:{if(!store((r[28]+uint32_t(0)),r[0]))return {};c.pc=0x800284cc;break;} // 901c0000 stw 0, 0(28)
case 0x800284cc:{if(!word((r[28]+uint32_t(228)),r[3]))return {};c.pc=0x800284d0;break;} // 807c00e4 lwz 3, 228(28)
case 0x800284d0:{compare(c,0,int64_t(r[3]),int64_t(0));c.pc=0x800284d4;break;} // 28030000 cmplwi 3, 0
case 0x800284d4:{c.pc=c.condition[2]?0x800284e4:0x800284d8;break;} // 41820010 bt 2, 0x800284e4
case 0x800284d8:{r[4]=r[28]+uint32_t(28);c.pc=0x800284dc;break;} // 389c001c addi 4, 28, 28
case 0x800284dc:{r[5]=uint32_t(1);c.pc=0x800284e0;break;} // 38a00001 li 5, 1
case 0x800284e0:{c.lr=0x800284e4;c.pc=0x80028130;break;} // 4bfffc51 bl 0x80028130
case 0x800284e4:{if(!word((r[28]+uint32_t(232)),r[3]))return {};c.pc=0x800284e8;break;} // 807c00e8 lwz 3, 232(28)
case 0x800284e8:{compare(c,0,int64_t(r[3]),int64_t(0));c.pc=0x800284ec;break;} // 28030000 cmplwi 3, 0
case 0x800284ec:{c.pc=c.condition[2]?0x80028530:0x800284f0;break;} // 41820044 bt 2, 0x80028530
case 0x800284f0:{r[4]=r[30];c.pc=0x800284f4;break;} // 7fc4f378 mr 4, 30
case 0x800284f4:{r[5]=r[31];c.pc=0x800284f8;break;} // 7fe5fb78 mr 5, 31
case 0x800284f8:{c.lr=0x800284fc;c.pc=0x80028130;break;} // 4bfffc39 bl 0x80028130
case 0x800284fc:{c.pc=0x80028530;break;} // 48000034 b 0x80028530
case 0x80028500:{if(!word((r[28]+uint32_t(228)),r[3]))return {};c.pc=0x80028504;break;} // 807c00e4 lwz 3, 228(28)
case 0x80028504:{compare(c,0,int64_t(r[3]),int64_t(0));c.pc=0x80028508;break;} // 28030000 cmplwi 3, 0
case 0x80028508:{c.pc=c.condition[2]?0x80028518:0x8002850c;break;} // 41820010 bt 2, 0x80028518
case 0x8002850c:{r[5]=r[31];c.pc=0x80028510;break;} // 7fe5fb78 mr 5, 31
case 0x80028510:{r[4]=r[28]+uint32_t(28);c.pc=0x80028514;break;} // 389c001c addi 4, 28, 28
case 0x80028514:{c.lr=0x80028518;c.pc=0x80028130;break;} // 4bfffc1d bl 0x80028130
case 0x80028518:{if(!word((r[28]+uint32_t(232)),r[3]))return {};c.pc=0x8002851c;break;} // 807c00e8 lwz 3, 232(28)
case 0x8002851c:{compare(c,0,int64_t(r[3]),int64_t(0));c.pc=0x80028520;break;} // 28030000 cmplwi 3, 0
case 0x80028520:{c.pc=c.condition[2]?0x80028530:0x80028524;break;} // 41820010 bt 2, 0x80028530
case 0x80028524:{r[4]=r[30];c.pc=0x80028528;break;} // 7fc4f378 mr 4, 30
case 0x80028528:{r[5]=r[31];c.pc=0x8002852c;break;} // 7fe5fb78 mr 5, 31
case 0x8002852c:{c.lr=0x80028530;c.pc=0x80028130;break;} // 4bfffc05 bl 0x80028130
case 0x80028530:{if(!word((r[1]+uint32_t(36)),r[0]))return {};c.pc=0x80028534;break;} // 80010024 lwz 0, 36(1)
case 0x80028534:{if(!word((r[1]+uint32_t(28)),r[31]))return {};c.pc=0x80028538;break;} // 83e1001c lwz 31, 28(1)
case 0x80028538:{if(!word((r[1]+uint32_t(24)),r[30]))return {};c.pc=0x8002853c;break;} // 83c10018 lwz 30, 24(1)
case 0x8002853c:{if(!word((r[1]+uint32_t(20)),r[29]))return {};c.pc=0x80028540;break;} // 83a10014 lwz 29, 20(1)
case 0x80028540:{if(!word((r[1]+uint32_t(16)),r[28]))return {};c.pc=0x80028544;break;} // 83810010 lwz 28, 16(1)
case 0x80028544:{c.lr=r[0];c.pc=0x80028548;break;} // 7c0803a6 mtlr 0
case 0x80028548:{r[1]=r[1]+uint32_t(32);c.pc=0x8002854c;break;} // 38210020 addi 1, 1, 32
case 0x8002854c:{c.pc=c.lr;break;} // 4e800020 blr 
