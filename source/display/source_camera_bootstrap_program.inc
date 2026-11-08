// Original US revision0 source. Five complete functions and one explicitly bounded CPU contribution.
// camInit 8000fd9c/ac8 SHA256 c63639c022820914ef06891f3f407fc38ba8ea681a48308fa1f061be9d6ca19d
// camEntryPersp 8000fac8/2d4 SHA256 8124e9f8dda6fb8323a6b001a4d5cf9ea9b7669bfdf8ee1e666f0e28f253931b
// C_MTXPerspective 8029e7e8/d0 SHA256 dc64426e64e14379313f2d35df8daf6f4fd47f5d476243e7436caf4ccbbaf7b5
// C_MTXOrtho 8029e8b8/98 SHA256 8a7c696bfe1df8825f9a8ed81b22e2410a03eaf226460acec844cd4b5bf1669b
// DEMOGetRenderModeObj 8002eb30/8 SHA256 05cc52dc2d70a4f25817360f6d9654e260346f3fdb0d456e2d422939ff67b462
// SelectedModeCopy 8002ed34/84 SHA256 9a8700100523a18295f19c61270185fb2206826f5f5d242c978266d7151da583
case 0x8000fd9c:{const auto target=(r[1]+uint32_t(-144));if(!store(target,r[1]))return {};r[1]=target;c.pc=0x8000fda0;break;} // 9421ff70 stwu 1, -144(1)
case 0x8000fda0:{r[0]=c.lr;c.pc=0x8000fda4;break;} // 7c0802a6 mflr 0
case 0x8000fda4:{r[4]=uint32_t(-32724)<<16;c.pc=0x8000fda8;break;} // 3c80802c lis 4, -32724
case 0x8000fda8:{r[3]=uint32_t(0);c.pc=0x8000fdac;break;} // 38600000 li 3, 0
case 0x8000fdac:{if(!store((r[1]+uint32_t(148)),r[0]))return {};c.pc=0x8000fdb0;break;} // 90010094 stw 0, 148(1)
case 0x8000fdb0:{if(!store((r[1]+uint32_t(140)),r[31]))return {};c.pc=0x8000fdb4;break;} // 93e1008c stw 31, 140(1)
case 0x8000fdb4:{if(!store((r[1]+uint32_t(136)),r[30]))return {};c.pc=0x8000fdb8;break;} // 93c10088 stw 30, 136(1)
case 0x8000fdb8:{r[30]=r[4]+uint32_t(-2600);c.pc=0x8000fdbc;break;} // 3bc4f5d8 addi 30, 4, -2600
case 0x8000fdbc:{r[4]=uint32_t(608);c.pc=0x8000fdc0;break;} // 38800260 li 4, 608
case 0x8000fdc0:{if(!external(0x800300f0,0x8000fdc4))return {};break;} // 48020331 bl 0x800300f0
case 0x8000fdc4:{r[31]=r[3];c.pc=0x8000fdc8;break;} // 7c7f1b78 mr 31, 3
case 0x8000fdc8:{r[4]=uint32_t(0);c.pc=0x8000fdcc;break;} // 38800000 li 4, 0
case 0x8000fdcc:{r[5]=uint32_t(608);c.pc=0x8000fdd0;break;} // 38a00260 li 5, 608
case 0x8000fdd0:{if(!external(0x800050b4,0x8000fdd4))return {};break;} // 4bff52e5 bl 0x800050b4
case 0x8000fdd4:{uint32_t v;if(!word((r[2]+uint32_t(-32132)),v))return {};f[1]=bits(double(single(v)));c.paired[1]=v;c.pc=0x8000fdd8;break;} // c022827c lfs 1, -32132(2)
case 0x8000fdd8:{r[3]=r[31]+uint32_t(348);c.pc=0x8000fddc;break;} // 387f015c addi 3, 31, 348
case 0x8000fddc:{uint32_t v;if(!word((r[2]+uint32_t(-32060)),v))return {};f[2]=bits(double(single(v)));c.paired[2]=v;c.pc=0x8000fde0;break;} // c04282c4 lfs 2, -32060(2)
case 0x8000fde0:{uint32_t v;if(!word((r[2]+uint32_t(-32056)),v))return {};f[3]=bits(double(single(v)));c.paired[3]=v;c.pc=0x8000fde4;break;} // c06282c8 lfs 3, -32056(2)
case 0x8000fde4:{uint32_t v;if(!word((r[2]+uint32_t(-32136)),v))return {};f[4]=bits(double(single(v)));c.paired[4]=v;c.pc=0x8000fde8;break;} // c0828278 lfs 4, -32136(2)
case 0x8000fde8:{uint32_t v;if(!word((r[2]+uint32_t(-32128)),v))return {};f[5]=bits(double(single(v)));c.paired[5]=v;c.pc=0x8000fdec;break;} // c0a28280 lfs 5, -32128(2)
case 0x8000fdec:{uint32_t v;if(!word((r[2]+uint32_t(-32112)),v))return {};f[6]=bits(double(single(v)));c.paired[6]=v;c.pc=0x8000fdf0;break;} // c0c28290 lfs 6, -32112(2)
case 0x8000fdf0:{c.lr=0x8000fdf4;c.pc=0x8029e8b8;break;} // 4828eac9 bl 0x8029e8b8
case 0x8000fdf4:{r[0]=uint32_t(1);c.pc=0x8000fdf8;break;} // 38000001 li 0, 1
case 0x8000fdf8:{uint32_t v;if(!word((r[2]+uint32_t(-32128)),v))return {};f[2]=bits(double(single(v)));c.paired[2]=v;c.pc=0x8000fdfc;break;} // c0428280 lfs 2, -32128(2)
case 0x8000fdfc:{if(!store((r[31]+uint32_t(412)),r[0]))return {};c.pc=0x8000fe00;break;} // 901f019c stw 0, 412(31)
case 0x8000fe00:{r[3]=r[31]+uint32_t(12);c.pc=0x8000fe04;break;} // 387f000c addi 3, 31, 12
case 0x8000fe04:{uint32_t v;if(!word((r[2]+uint32_t(-32112)),v))return {};f[1]=bits(double(single(v)));c.paired[1]=v;c.pc=0x8000fe08;break;} // c0228290 lfs 1, -32112(2)
case 0x8000fe08:{r[4]=r[31]+uint32_t(24);c.pc=0x8000fe0c;break;} // 389f0018 addi 4, 31, 24
case 0x8000fe0c:{if(!word((r[13]+uint32_t(-32624)),r[5]))return {};c.pc=0x8000fe10;break;} // 80ad8090 lwz 5, -32624(13)
case 0x8000fe10:{if(!word((r[13]+uint32_t(-32620)),r[0]))return {};c.pc=0x8000fe14;break;} // 800d8094 lwz 0, -32620(13)
case 0x8000fe14:{uint32_t v;if(!word((r[2]+uint32_t(-32140)),v))return {};f[0]=bits(double(single(v)));c.paired[0]=v;c.pc=0x8000fe18;break;} // c0028274 lfs 0, -32140(2)
case 0x8000fe18:{if(!store((r[31]+uint32_t(12)),r[5]))return {};c.pc=0x8000fe1c;break;} // 90bf000c stw 5, 12(31)
case 0x8000fe1c:{if(!store((r[31]+uint32_t(16)),r[0]))return {};c.pc=0x8000fe20;break;} // 901f0010 stw 0, 16(31)
case 0x8000fe20:{if(!word((r[13]+uint32_t(-32616)),r[0]))return {};c.pc=0x8000fe24;break;} // 800d8098 lwz 0, -32616(13)
case 0x8000fe24:{if(!store((r[31]+uint32_t(20)),r[0]))return {};c.pc=0x8000fe28;break;} // 901f0014 stw 0, 20(31)
case 0x8000fe28:{if(!word((r[13]+uint32_t(-32612)),r[5]))return {};c.pc=0x8000fe2c;break;} // 80ad809c lwz 5, -32612(13)
case 0x8000fe2c:{if(!word((r[13]+uint32_t(-32608)),r[0]))return {};c.pc=0x8000fe30;break;} // 800d80a0 lwz 0, -32608(13)
case 0x8000fe30:{if(!store((r[31]+uint32_t(24)),r[5]))return {};c.pc=0x8000fe34;break;} // 90bf0018 stw 5, 24(31)
case 0x8000fe34:{if(!store((r[31]+uint32_t(28)),r[0]))return {};c.pc=0x8000fe38;break;} // 901f001c stw 0, 28(31)
case 0x8000fe38:{if(!word((r[13]+uint32_t(-32604)),r[0]))return {};c.pc=0x8000fe3c;break;} // 800d80a4 lwz 0, -32604(13)
case 0x8000fe3c:{if(!store((r[31]+uint32_t(32)),r[0]))return {};c.pc=0x8000fe40;break;} // 901f0020 stw 0, 32(31)
case 0x8000fe40:{if(!word((r[13]+uint32_t(-32600)),r[5]))return {};c.pc=0x8000fe44;break;} // 80ad80a8 lwz 5, -32600(13)
case 0x8000fe44:{if(!word((r[13]+uint32_t(-32596)),r[0]))return {};c.pc=0x8000fe48;break;} // 800d80ac lwz 0, -32596(13)
case 0x8000fe48:{if(!store((r[31]+uint32_t(36)),r[5]))return {};c.pc=0x8000fe4c;break;} // 90bf0024 stw 5, 36(31)
case 0x8000fe4c:{if(!store((r[31]+uint32_t(40)),r[0]))return {};c.pc=0x8000fe50;break;} // 901f0028 stw 0, 40(31)
case 0x8000fe50:{if(!word((r[13]+uint32_t(-32592)),r[0]))return {};c.pc=0x8000fe54;break;} // 800d80b0 lwz 0, -32592(13)
case 0x8000fe54:{if(!store((r[31]+uint32_t(44)),r[0]))return {};c.pc=0x8000fe58;break;} // 901f002c stw 0, 44(31)
case 0x8000fe58:{if(!store((r[31]+uint32_t(48)),singleBits(scalar(f[2]))))return {};c.pc=0x8000fe5c;break;} // d05f0030 stfs 2, 48(31)
case 0x8000fe5c:{if(!store((r[31]+uint32_t(52)),singleBits(scalar(f[1]))))return {};c.pc=0x8000fe60;break;} // d03f0034 stfs 1, 52(31)
case 0x8000fe60:{if(!store((r[31]+uint32_t(276)),singleBits(scalar(f[0]))))return {};c.pc=0x8000fe64;break;} // d01f0114 stfs 0, 276(31)
case 0x8000fe64:{if(!external(0x8029eeb4,0x8000fe68))return {};break;} // 4828f051 bl 0x8029eeb4
case 0x8000fe68:{if(!store((r[31]+uint32_t(280)),singleBits(scalar(f[1]))))return {};c.pc=0x8000fe6c;break;} // d03f0118 stfs 1, 280(31)
case 0x8000fe6c:{r[5]=uint32_t(0);c.pc=0x8000fe70;break;} // 38a00000 li 5, 0
case 0x8000fe70:{uint32_t v;if(!word((r[2]+uint32_t(-32140)),v))return {};f[2]=bits(double(single(v)));c.paired[2]=v;c.pc=0x8000fe74;break;} // c0428274 lfs 2, -32140(2)
case 0x8000fe74:{r[0]=uint32_t(8);c.pc=0x8000fe78;break;} // 38000008 li 0, 8
case 0x8000fe78:{if(!store((r[31]+uint32_t(492)),r[5]))return {};c.pc=0x8000fe7c;break;} // 90bf01ec stw 5, 492(31)
case 0x8000fe7c:{r[3]=r[31]+uint32_t(596);c.pc=0x8000fe80;break;} // 387f0254 addi 3, 31, 596
case 0x8000fe80:{uint32_t v;if(!word((r[2]+uint32_t(-32104)),v))return {};f[1]=bits(double(single(v)));c.paired[1]=v;c.pc=0x8000fe84;break;} // c0228298 lfs 1, -32104(2)
case 0x8000fe84:{r[4]=r[2]+uint32_t(-32052);c.pc=0x8000fe88;break;} // 388282cc addi 4, 2, -32052
case 0x8000fe88:{if(!s.storeHalf||!s.storeHalf(s.user,(r[31]+uint32_t(2)),uint16_t(r[5])))return {};c.pc=0x8000fe8c;break;} // b0bf0002 sth 5, 2(31)
case 0x8000fe8c:{uint32_t v;if(!word((r[2]+uint32_t(-32116)),v))return {};f[0]=bits(double(single(v)));c.paired[0]=v;c.pc=0x8000fe90;break;} // c002828c lfs 0, -32116(2)
case 0x8000fe90:{if(!store((r[31]+uint32_t(332)),singleBits(scalar(f[2]))))return {};c.pc=0x8000fe94;break;} // d05f014c stfs 2, 332(31)
case 0x8000fe94:{if(!store((r[31]+uint32_t(344)),singleBits(scalar(f[2]))))return {};c.pc=0x8000fe98;break;} // d05f0158 stfs 2, 344(31)
case 0x8000fe98:{if(!store((r[31]+uint32_t(340)),singleBits(scalar(f[2]))))return {};c.pc=0x8000fe9c;break;} // d05f0154 stfs 2, 340(31)
case 0x8000fe9c:{if(!store((r[31]+uint32_t(336)),singleBits(scalar(f[2]))))return {};c.pc=0x8000fea0;break;} // d05f0150 stfs 2, 336(31)
case 0x8000fea0:{if(!store((r[31]+uint32_t(60)),singleBits(scalar(f[1]))))return {};c.pc=0x8000fea4;break;} // d03f003c stfs 1, 60(31)
case 0x8000fea4:{if(!store((r[31]+uint32_t(56)),singleBits(scalar(f[0]))))return {};c.pc=0x8000fea8;break;} // d01f0038 stfs 0, 56(31)
case 0x8000fea8:{if(!store((r[31]+uint32_t(488)),r[0]))return {};c.pc=0x8000feac;break;} // 901f01e8 stw 0, 488(31)
case 0x8000feac:{if(!external(0x8026d5fc,0x8000feb0))return {};break;} // 4825d751 bl 0x8026d5fc
case 0x8000feb0:{r[0]=uint32_t(0);c.pc=0x8000feb4;break;} // 38000000 li 0, 0
case 0x8000feb4:{if(!s.storeHalf||!s.storeHalf(s.user,(r[31]+uint32_t(244)),uint16_t(r[0])))return {};c.pc=0x8000feb8;break;} // b01f00f4 sth 0, 244(31)
case 0x8000feb8:{if(!s.storeHalf||!s.storeHalf(s.user,(r[31]+uint32_t(246)),uint16_t(r[0])))return {};c.pc=0x8000febc;break;} // b01f00f6 sth 0, 246(31)
case 0x8000febc:{c.lr=0x8000fec0;c.pc=0x8002eb30;break;} // 4801ec75 bl 0x8002eb30
case 0x8000fec0:{if(!half((r[3]+uint32_t(4)),r[0]))return {};c.pc=0x8000fec4;break;} // a0030004 lhz 0, 4(3)
case 0x8000fec4:{if(!s.storeHalf||!s.storeHalf(s.user,(r[31]+uint32_t(248)),uint16_t(r[0])))return {};c.pc=0x8000fec8;break;} // b01f00f8 sth 0, 248(31)
case 0x8000fec8:{c.lr=0x8000fecc;c.pc=0x8002eb30;break;} // 4801ec69 bl 0x8002eb30
case 0x8000fecc:{if(!half((r[3]+uint32_t(6)),r[0]))return {};c.pc=0x8000fed0;break;} // a0030006 lhz 0, 6(3)
case 0x8000fed0:{uint32_t v;if(!word((r[2]+uint32_t(-32140)),v))return {};f[0]=bits(double(single(v)));c.paired[0]=v;c.pc=0x8000fed4;break;} // c0028274 lfs 0, -32140(2)
case 0x8000fed4:{if(!s.storeHalf||!s.storeHalf(s.user,(r[31]+uint32_t(250)),uint16_t(r[0])))return {};c.pc=0x8000fed8;break;} // b01f00fa sth 0, 250(31)
case 0x8000fed8:{if(!store((r[31]+uint32_t(252)),singleBits(scalar(f[0]))))return {};c.pc=0x8000fedc;break;} // d01f00fc stfs 0, 252(31)
case 0x8000fedc:{if(!store((r[31]+uint32_t(256)),singleBits(scalar(f[0]))))return {};c.pc=0x8000fee0;break;} // d01f0100 stfs 0, 256(31)
case 0x8000fee0:{c.lr=0x8000fee4;c.pc=0x8002eb30;break;} // 4801ec51 bl 0x8002eb30
case 0x8000fee4:{if(!half((r[3]+uint32_t(4)),r[3]))return {};c.pc=0x8000fee8;break;} // a0630004 lhz 3, 4(3)
case 0x8000fee8:{r[0]=uint32_t(17200)<<16;c.pc=0x8000feec;break;} // 3c004330 lis 0, 17200
case 0x8000feec:{if(!store((r[1]+uint32_t(56)),r[0]))return {};c.pc=0x8000fef0;break;} // 90010038 stw 0, 56(1)
case 0x8000fef0:{if(!c.step){if(!word((r[30]+uint32_t(104)),c.transfer))return {};c.step=1;}uint32_t low;if(!word((r[30]+uint32_t(104))+4,low))return {};f[1]=(uint64_t(c.transfer)<<32)|low;c.step=0;c.pc=0x8000fef4;break;} // c83e0068 lfd 1, 104(30)
case 0x8000fef4:{if(!store((r[1]+uint32_t(60)),r[3]))return {};c.pc=0x8000fef8;break;} // 9061003c stw 3, 60(1)
case 0x8000fef8:{if(!c.step){if(!word((r[1]+uint32_t(56)),c.transfer))return {};c.step=1;}uint32_t low;if(!word((r[1]+uint32_t(56))+4,low))return {};f[0]=(uint64_t(c.transfer)<<32)|low;c.step=0;c.pc=0x8000fefc;break;} // c8010038 lfd 0, 56(1)
case 0x8000fefc:{f[0]=rounded(scalar(f[0])-scalar(f[1]));c.pc=0x8000ff00;break;} // ec000828 fsubs 0, 0, 1
case 0x8000ff00:{if(!store((r[31]+uint32_t(260)),singleBits(scalar(f[0]))))return {};c.pc=0x8000ff04;break;} // d01f0104 stfs 0, 260(31)
case 0x8000ff04:{c.lr=0x8000ff08;c.pc=0x8002eb30;break;} // 4801ec2d bl 0x8002eb30
case 0x8000ff08:{if(!half((r[3]+uint32_t(6)),r[4]))return {};c.pc=0x8000ff0c;break;} // a0830006 lhz 4, 6(3)
case 0x8000ff0c:{r[0]=uint32_t(17200)<<16;c.pc=0x8000ff10;break;} // 3c004330 lis 0, 17200
case 0x8000ff10:{r[3]=uint32_t(-32707)<<16;c.pc=0x8000ff14;break;} // 3c60803d lis 3, -32707
case 0x8000ff14:{if(!store((r[1]+uint32_t(64)),r[0]))return {};c.pc=0x8000ff18;break;} // 90010040 stw 0, 64(1)
case 0x8000ff18:{if(!c.step){if(!word((r[30]+uint32_t(104)),c.transfer))return {};c.step=1;}uint32_t low;if(!word((r[30]+uint32_t(104))+4,low))return {};f[3]=(uint64_t(c.transfer)<<32)|low;c.step=0;c.pc=0x8000ff1c;break;} // c87e0068 lfd 3, 104(30)
case 0x8000ff1c:{r[5]=r[3]+uint32_t(-27144);c.pc=0x8000ff20;break;} // 38a395f8 addi 5, 3, -27144
case 0x8000ff20:{if(!store((r[1]+uint32_t(68)),r[4]))return {};c.pc=0x8000ff24;break;} // 90810044 stw 4, 68(1)
case 0x8000ff24:{r[3]=uint32_t(0);c.pc=0x8000ff28;break;} // 38600000 li 3, 0
case 0x8000ff28:{uint32_t v;if(!word((r[2]+uint32_t(-32140)),v))return {};f[1]=bits(double(single(v)));c.paired[1]=v;c.pc=0x8000ff2c;break;} // c0228274 lfs 1, -32140(2)
case 0x8000ff2c:{r[4]=uint32_t(608);c.pc=0x8000ff30;break;} // 38800260 li 4, 608
case 0x8000ff30:{if(!c.step){if(!word((r[1]+uint32_t(64)),c.transfer))return {};c.step=1;}uint32_t low;if(!word((r[1]+uint32_t(64))+4,low))return {};f[2]=(uint64_t(c.transfer)<<32)|low;c.step=0;c.pc=0x8000ff34;break;} // c8410040 lfd 2, 64(1)
case 0x8000ff34:{uint32_t v;if(!word((r[2]+uint32_t(-32128)),v))return {};f[0]=bits(double(single(v)));c.paired[0]=v;c.pc=0x8000ff38;break;} // c0028280 lfs 0, -32128(2)
case 0x8000ff38:{f[2]=rounded(scalar(f[2])-scalar(f[3]));c.pc=0x8000ff3c;break;} // ec421828 fsubs 2, 2, 3
case 0x8000ff3c:{if(!store((r[31]+uint32_t(264)),singleBits(scalar(f[2]))))return {};c.pc=0x8000ff40;break;} // d05f0108 stfs 2, 264(31)
case 0x8000ff40:{if(!store((r[31]+uint32_t(268)),singleBits(scalar(f[1]))))return {};c.pc=0x8000ff44;break;} // d03f010c stfs 1, 268(31)
case 0x8000ff44:{if(!store((r[31]+uint32_t(272)),singleBits(scalar(f[0]))))return {};c.pc=0x8000ff48;break;} // d01f0110 stfs 0, 272(31)
case 0x8000ff48:{if(!store((r[5]+uint32_t(0)),r[31]))return {};c.pc=0x8000ff4c;break;} // 93e50000 stw 31, 0(5)
case 0x8000ff4c:{if(!external(0x800300f0,0x8000ff50))return {};break;} // 480201a5 bl 0x800300f0
case 0x8000ff50:{r[31]=r[3];c.pc=0x8000ff54;break;} // 7c7f1b78 mr 31, 3
case 0x8000ff54:{r[4]=uint32_t(0);c.pc=0x8000ff58;break;} // 38800000 li 4, 0
case 0x8000ff58:{r[5]=uint32_t(608);c.pc=0x8000ff5c;break;} // 38a00260 li 5, 608
case 0x8000ff5c:{if(!external(0x800050b4,0x8000ff60))return {};break;} // 4bff5159 bl 0x800050b4
case 0x8000ff60:{uint32_t v;if(!word((r[2]+uint32_t(-32140)),v))return {};f[1]=bits(double(single(v)));c.paired[1]=v;c.pc=0x8000ff64;break;} // c0228274 lfs 1, -32140(2)
case 0x8000ff64:{r[3]=r[31]+uint32_t(348);c.pc=0x8000ff68;break;} // 387f015c addi 3, 31, 348
case 0x8000ff68:{uint32_t v;if(!word((r[2]+uint32_t(-32096)),v))return {};f[2]=bits(double(single(v)));c.paired[2]=v;c.pc=0x8000ff6c;break;} // c04282a0 lfs 2, -32096(2)
case 0x8000ff6c:{f[3]=f[1];c.pc=0x8000ff70;break;} // fc600890 fmr 3, 1
case 0x8000ff70:{uint32_t v;if(!word((r[2]+uint32_t(-32100)),v))return {};f[4]=bits(double(single(v)));c.paired[4]=v;c.pc=0x8000ff74;break;} // c082829c lfs 4, -32100(2)
case 0x8000ff74:{uint32_t v;if(!word((r[2]+uint32_t(-32128)),v))return {};f[5]=bits(double(single(v)));c.paired[5]=v;c.pc=0x8000ff78;break;} // c0a28280 lfs 5, -32128(2)
case 0x8000ff78:{uint32_t v;if(!word((r[2]+uint32_t(-32112)),v))return {};f[6]=bits(double(single(v)));c.paired[6]=v;c.pc=0x8000ff7c;break;} // c0c28290 lfs 6, -32112(2)
case 0x8000ff7c:{c.lr=0x8000ff80;c.pc=0x8029e8b8;break;} // 4828e93d bl 0x8029e8b8
case 0x8000ff80:{r[0]=uint32_t(1);c.pc=0x8000ff84;break;} // 38000001 li 0, 1
case 0x8000ff84:{uint32_t v;if(!word((r[2]+uint32_t(-32128)),v))return {};f[2]=bits(double(single(v)));c.paired[2]=v;c.pc=0x8000ff88;break;} // c0428280 lfs 2, -32128(2)
case 0x8000ff88:{if(!store((r[31]+uint32_t(412)),r[0]))return {};c.pc=0x8000ff8c;break;} // 901f019c stw 0, 412(31)
case 0x8000ff8c:{r[3]=r[31]+uint32_t(12);c.pc=0x8000ff90;break;} // 387f000c addi 3, 31, 12
case 0x8000ff90:{uint32_t v;if(!word((r[2]+uint32_t(-32112)),v))return {};f[1]=bits(double(single(v)));c.paired[1]=v;c.pc=0x8000ff94;break;} // c0228290 lfs 1, -32112(2)
case 0x8000ff94:{r[4]=r[31]+uint32_t(24);c.pc=0x8000ff98;break;} // 389f0018 addi 4, 31, 24
case 0x8000ff98:{if(!word((r[13]+uint32_t(-32624)),r[5]))return {};c.pc=0x8000ff9c;break;} // 80ad8090 lwz 5, -32624(13)
case 0x8000ff9c:{if(!word((r[13]+uint32_t(-32620)),r[0]))return {};c.pc=0x8000ffa0;break;} // 800d8094 lwz 0, -32620(13)
case 0x8000ffa0:{uint32_t v;if(!word((r[2]+uint32_t(-32140)),v))return {};f[0]=bits(double(single(v)));c.paired[0]=v;c.pc=0x8000ffa4;break;} // c0028274 lfs 0, -32140(2)
case 0x8000ffa4:{if(!store((r[31]+uint32_t(12)),r[5]))return {};c.pc=0x8000ffa8;break;} // 90bf000c stw 5, 12(31)
case 0x8000ffa8:{if(!store((r[31]+uint32_t(16)),r[0]))return {};c.pc=0x8000ffac;break;} // 901f0010 stw 0, 16(31)
case 0x8000ffac:{if(!word((r[13]+uint32_t(-32616)),r[0]))return {};c.pc=0x8000ffb0;break;} // 800d8098 lwz 0, -32616(13)
case 0x8000ffb0:{if(!store((r[31]+uint32_t(20)),r[0]))return {};c.pc=0x8000ffb4;break;} // 901f0014 stw 0, 20(31)
case 0x8000ffb4:{if(!word((r[13]+uint32_t(-32612)),r[5]))return {};c.pc=0x8000ffb8;break;} // 80ad809c lwz 5, -32612(13)
case 0x8000ffb8:{if(!word((r[13]+uint32_t(-32608)),r[0]))return {};c.pc=0x8000ffbc;break;} // 800d80a0 lwz 0, -32608(13)
case 0x8000ffbc:{if(!store((r[31]+uint32_t(24)),r[5]))return {};c.pc=0x8000ffc0;break;} // 90bf0018 stw 5, 24(31)
case 0x8000ffc0:{if(!store((r[31]+uint32_t(28)),r[0]))return {};c.pc=0x8000ffc4;break;} // 901f001c stw 0, 28(31)
case 0x8000ffc4:{if(!word((r[13]+uint32_t(-32604)),r[0]))return {};c.pc=0x8000ffc8;break;} // 800d80a4 lwz 0, -32604(13)
case 0x8000ffc8:{if(!store((r[31]+uint32_t(32)),r[0]))return {};c.pc=0x8000ffcc;break;} // 901f0020 stw 0, 32(31)
case 0x8000ffcc:{if(!word((r[13]+uint32_t(-32600)),r[5]))return {};c.pc=0x8000ffd0;break;} // 80ad80a8 lwz 5, -32600(13)
case 0x8000ffd0:{if(!word((r[13]+uint32_t(-32596)),r[0]))return {};c.pc=0x8000ffd4;break;} // 800d80ac lwz 0, -32596(13)
case 0x8000ffd4:{if(!store((r[31]+uint32_t(36)),r[5]))return {};c.pc=0x8000ffd8;break;} // 90bf0024 stw 5, 36(31)
case 0x8000ffd8:{if(!store((r[31]+uint32_t(40)),r[0]))return {};c.pc=0x8000ffdc;break;} // 901f0028 stw 0, 40(31)
case 0x8000ffdc:{if(!word((r[13]+uint32_t(-32592)),r[0]))return {};c.pc=0x8000ffe0;break;} // 800d80b0 lwz 0, -32592(13)
case 0x8000ffe0:{if(!store((r[31]+uint32_t(44)),r[0]))return {};c.pc=0x8000ffe4;break;} // 901f002c stw 0, 44(31)
case 0x8000ffe4:{if(!store((r[31]+uint32_t(48)),singleBits(scalar(f[2]))))return {};c.pc=0x8000ffe8;break;} // d05f0030 stfs 2, 48(31)
case 0x8000ffe8:{if(!store((r[31]+uint32_t(52)),singleBits(scalar(f[1]))))return {};c.pc=0x8000ffec;break;} // d03f0034 stfs 1, 52(31)
case 0x8000ffec:{if(!store((r[31]+uint32_t(276)),singleBits(scalar(f[0]))))return {};c.pc=0x8000fff0;break;} // d01f0114 stfs 0, 276(31)
case 0x8000fff0:{if(!external(0x8029eeb4,0x8000fff4))return {};break;} // 4828eec5 bl 0x8029eeb4
case 0x8000fff4:{if(!store((r[31]+uint32_t(280)),singleBits(scalar(f[1]))))return {};c.pc=0x8000fff8;break;} // d03f0118 stfs 1, 280(31)
case 0x8000fff8:{r[5]=uint32_t(0);c.pc=0x8000fffc;break;} // 38a00000 li 5, 0
case 0x8000fffc:{uint32_t v;if(!word((r[2]+uint32_t(-32140)),v))return {};f[2]=bits(double(single(v)));c.paired[2]=v;c.pc=0x80010000;break;} // c0428274 lfs 2, -32140(2)
case 0x80010000:{r[0]=uint32_t(8);c.pc=0x80010004;break;} // 38000008 li 0, 8
case 0x80010004:{if(!store((r[31]+uint32_t(492)),r[5]))return {};c.pc=0x80010008;break;} // 90bf01ec stw 5, 492(31)
case 0x80010008:{r[3]=r[31]+uint32_t(596);c.pc=0x8001000c;break;} // 387f0254 addi 3, 31, 596
case 0x8001000c:{uint32_t v;if(!word((r[2]+uint32_t(-32104)),v))return {};f[1]=bits(double(single(v)));c.paired[1]=v;c.pc=0x80010010;break;} // c0228298 lfs 1, -32104(2)
case 0x80010010:{r[4]=r[2]+uint32_t(-32048);c.pc=0x80010014;break;} // 388282d0 addi 4, 2, -32048
case 0x80010014:{if(!s.storeHalf||!s.storeHalf(s.user,(r[31]+uint32_t(2)),uint16_t(r[5])))return {};c.pc=0x80010018;break;} // b0bf0002 sth 5, 2(31)
case 0x80010018:{uint32_t v;if(!word((r[2]+uint32_t(-32116)),v))return {};f[0]=bits(double(single(v)));c.paired[0]=v;c.pc=0x8001001c;break;} // c002828c lfs 0, -32116(2)
case 0x8001001c:{if(!store((r[31]+uint32_t(332)),singleBits(scalar(f[2]))))return {};c.pc=0x80010020;break;} // d05f014c stfs 2, 332(31)
case 0x80010020:{if(!store((r[31]+uint32_t(344)),singleBits(scalar(f[2]))))return {};c.pc=0x80010024;break;} // d05f0158 stfs 2, 344(31)
case 0x80010024:{if(!store((r[31]+uint32_t(340)),singleBits(scalar(f[2]))))return {};c.pc=0x80010028;break;} // d05f0154 stfs 2, 340(31)
case 0x80010028:{if(!store((r[31]+uint32_t(336)),singleBits(scalar(f[2]))))return {};c.pc=0x8001002c;break;} // d05f0150 stfs 2, 336(31)
case 0x8001002c:{if(!store((r[31]+uint32_t(60)),singleBits(scalar(f[1]))))return {};c.pc=0x80010030;break;} // d03f003c stfs 1, 60(31)
case 0x80010030:{if(!store((r[31]+uint32_t(56)),singleBits(scalar(f[0]))))return {};c.pc=0x80010034;break;} // d01f0038 stfs 0, 56(31)
case 0x80010034:{if(!store((r[31]+uint32_t(488)),r[0]))return {};c.pc=0x80010038;break;} // 901f01e8 stw 0, 488(31)
case 0x80010038:{if(!external(0x8026d5fc,0x8001003c))return {};break;} // 4825d5c5 bl 0x8026d5fc
case 0x8001003c:{r[0]=uint32_t(0);c.pc=0x80010040;break;} // 38000000 li 0, 0
case 0x80010040:{if(!s.storeHalf||!s.storeHalf(s.user,(r[31]+uint32_t(244)),uint16_t(r[0])))return {};c.pc=0x80010044;break;} // b01f00f4 sth 0, 244(31)
case 0x80010044:{if(!s.storeHalf||!s.storeHalf(s.user,(r[31]+uint32_t(246)),uint16_t(r[0])))return {};c.pc=0x80010048;break;} // b01f00f6 sth 0, 246(31)
case 0x80010048:{c.lr=0x8001004c;c.pc=0x8002eb30;break;} // 4801eae9 bl 0x8002eb30
case 0x8001004c:{if(!half((r[3]+uint32_t(4)),r[0]))return {};c.pc=0x80010050;break;} // a0030004 lhz 0, 4(3)
case 0x80010050:{if(!s.storeHalf||!s.storeHalf(s.user,(r[31]+uint32_t(248)),uint16_t(r[0])))return {};c.pc=0x80010054;break;} // b01f00f8 sth 0, 248(31)
case 0x80010054:{c.lr=0x80010058;c.pc=0x8002eb30;break;} // 4801eadd bl 0x8002eb30
case 0x80010058:{if(!half((r[3]+uint32_t(6)),r[0]))return {};c.pc=0x8001005c;break;} // a0030006 lhz 0, 6(3)
case 0x8001005c:{uint32_t v;if(!word((r[2]+uint32_t(-32140)),v))return {};f[0]=bits(double(single(v)));c.paired[0]=v;c.pc=0x80010060;break;} // c0028274 lfs 0, -32140(2)
case 0x80010060:{if(!s.storeHalf||!s.storeHalf(s.user,(r[31]+uint32_t(250)),uint16_t(r[0])))return {};c.pc=0x80010064;break;} // b01f00fa sth 0, 250(31)
case 0x80010064:{if(!store((r[31]+uint32_t(252)),singleBits(scalar(f[0]))))return {};c.pc=0x80010068;break;} // d01f00fc stfs 0, 252(31)
case 0x80010068:{if(!store((r[31]+uint32_t(256)),singleBits(scalar(f[0]))))return {};c.pc=0x8001006c;break;} // d01f0100 stfs 0, 256(31)
case 0x8001006c:{c.lr=0x80010070;c.pc=0x8002eb30;break;} // 4801eac5 bl 0x8002eb30
case 0x80010070:{if(!half((r[3]+uint32_t(4)),r[3]))return {};c.pc=0x80010074;break;} // a0630004 lhz 3, 4(3)
case 0x80010074:{r[0]=uint32_t(17200)<<16;c.pc=0x80010078;break;} // 3c004330 lis 0, 17200
case 0x80010078:{if(!store((r[1]+uint32_t(72)),r[0]))return {};c.pc=0x8001007c;break;} // 90010048 stw 0, 72(1)
case 0x8001007c:{if(!c.step){if(!word((r[30]+uint32_t(104)),c.transfer))return {};c.step=1;}uint32_t low;if(!word((r[30]+uint32_t(104))+4,low))return {};f[1]=(uint64_t(c.transfer)<<32)|low;c.step=0;c.pc=0x80010080;break;} // c83e0068 lfd 1, 104(30)
case 0x80010080:{if(!store((r[1]+uint32_t(76)),r[3]))return {};c.pc=0x80010084;break;} // 9061004c stw 3, 76(1)
case 0x80010084:{if(!c.step){if(!word((r[1]+uint32_t(72)),c.transfer))return {};c.step=1;}uint32_t low;if(!word((r[1]+uint32_t(72))+4,low))return {};f[0]=(uint64_t(c.transfer)<<32)|low;c.step=0;c.pc=0x80010088;break;} // c8010048 lfd 0, 72(1)
case 0x80010088:{f[0]=rounded(scalar(f[0])-scalar(f[1]));c.pc=0x8001008c;break;} // ec000828 fsubs 0, 0, 1
case 0x8001008c:{if(!store((r[31]+uint32_t(260)),singleBits(scalar(f[0]))))return {};c.pc=0x80010090;break;} // d01f0104 stfs 0, 260(31)
case 0x80010090:{c.lr=0x80010094;c.pc=0x8002eb30;break;} // 4801eaa1 bl 0x8002eb30
case 0x80010094:{if(!half((r[3]+uint32_t(6)),r[4]))return {};c.pc=0x80010098;break;} // a0830006 lhz 4, 6(3)
case 0x80010098:{r[0]=uint32_t(17200)<<16;c.pc=0x8001009c;break;} // 3c004330 lis 0, 17200
case 0x8001009c:{r[3]=uint32_t(-32707)<<16;c.pc=0x800100a0;break;} // 3c60803d lis 3, -32707
case 0x800100a0:{if(!store((r[1]+uint32_t(80)),r[0]))return {};c.pc=0x800100a4;break;} // 90010050 stw 0, 80(1)
case 0x800100a4:{if(!c.step){if(!word((r[30]+uint32_t(104)),c.transfer))return {};c.step=1;}uint32_t low;if(!word((r[30]+uint32_t(104))+4,low))return {};f[3]=(uint64_t(c.transfer)<<32)|low;c.step=0;c.pc=0x800100a8;break;} // c87e0068 lfd 3, 104(30)
case 0x800100a8:{r[5]=r[3]+uint32_t(-27144);c.pc=0x800100ac;break;} // 38a395f8 addi 5, 3, -27144
case 0x800100ac:{if(!store((r[1]+uint32_t(84)),r[4]))return {};c.pc=0x800100b0;break;} // 90810054 stw 4, 84(1)
case 0x800100b0:{r[3]=uint32_t(0);c.pc=0x800100b4;break;} // 38600000 li 3, 0
case 0x800100b4:{uint32_t v;if(!word((r[2]+uint32_t(-32140)),v))return {};f[1]=bits(double(single(v)));c.paired[1]=v;c.pc=0x800100b8;break;} // c0228274 lfs 1, -32140(2)
case 0x800100b8:{r[4]=uint32_t(608);c.pc=0x800100bc;break;} // 38800260 li 4, 608
case 0x800100bc:{if(!c.step){if(!word((r[1]+uint32_t(80)),c.transfer))return {};c.step=1;}uint32_t low;if(!word((r[1]+uint32_t(80))+4,low))return {};f[2]=(uint64_t(c.transfer)<<32)|low;c.step=0;c.pc=0x800100c0;break;} // c8410050 lfd 2, 80(1)
case 0x800100c0:{uint32_t v;if(!word((r[2]+uint32_t(-32128)),v))return {};f[0]=bits(double(single(v)));c.paired[0]=v;c.pc=0x800100c4;break;} // c0028280 lfs 0, -32128(2)
case 0x800100c4:{f[2]=rounded(scalar(f[2])-scalar(f[3]));c.pc=0x800100c8;break;} // ec421828 fsubs 2, 2, 3
case 0x800100c8:{if(!store((r[31]+uint32_t(264)),singleBits(scalar(f[2]))))return {};c.pc=0x800100cc;break;} // d05f0108 stfs 2, 264(31)
case 0x800100cc:{if(!store((r[31]+uint32_t(268)),singleBits(scalar(f[1]))))return {};c.pc=0x800100d0;break;} // d03f010c stfs 1, 268(31)
case 0x800100d0:{if(!store((r[31]+uint32_t(272)),singleBits(scalar(f[0]))))return {};c.pc=0x800100d4;break;} // d01f0110 stfs 0, 272(31)
case 0x800100d4:{if(!store((r[5]+uint32_t(4)),r[31]))return {};c.pc=0x800100d8;break;} // 93e50004 stw 31, 4(5)
case 0x800100d8:{if(!external(0x800300f0,0x800100dc))return {};break;} // 48020019 bl 0x800300f0
case 0x800100dc:{r[31]=r[3];c.pc=0x800100e0;break;} // 7c7f1b78 mr 31, 3
case 0x800100e0:{r[4]=uint32_t(0);c.pc=0x800100e4;break;} // 38800000 li 4, 0
case 0x800100e4:{r[5]=uint32_t(608);c.pc=0x800100e8;break;} // 38a00260 li 5, 608
case 0x800100e8:{if(!external(0x800050b4,0x800100ec))return {};break;} // 4bff4fcd bl 0x800050b4
case 0x800100ec:{uint32_t v;if(!word((r[2]+uint32_t(-32140)),v))return {};f[1]=bits(double(single(v)));c.paired[1]=v;c.pc=0x800100f0;break;} // c0228274 lfs 1, -32140(2)
case 0x800100f0:{r[3]=r[31]+uint32_t(348);c.pc=0x800100f4;break;} // 387f015c addi 3, 31, 348
case 0x800100f4:{uint32_t v;if(!word((r[2]+uint32_t(-32096)),v))return {};f[2]=bits(double(single(v)));c.paired[2]=v;c.pc=0x800100f8;break;} // c04282a0 lfs 2, -32096(2)
case 0x800100f8:{f[3]=f[1];c.pc=0x800100fc;break;} // fc600890 fmr 3, 1
case 0x800100fc:{uint32_t v;if(!word((r[2]+uint32_t(-32100)),v))return {};f[4]=bits(double(single(v)));c.paired[4]=v;c.pc=0x80010100;break;} // c082829c lfs 4, -32100(2)
case 0x80010100:{uint32_t v;if(!word((r[2]+uint32_t(-32128)),v))return {};f[5]=bits(double(single(v)));c.paired[5]=v;c.pc=0x80010104;break;} // c0a28280 lfs 5, -32128(2)
case 0x80010104:{uint32_t v;if(!word((r[2]+uint32_t(-32112)),v))return {};f[6]=bits(double(single(v)));c.paired[6]=v;c.pc=0x80010108;break;} // c0c28290 lfs 6, -32112(2)
case 0x80010108:{c.lr=0x8001010c;c.pc=0x8029e8b8;break;} // 4828e7b1 bl 0x8029e8b8
case 0x8001010c:{r[0]=uint32_t(1);c.pc=0x80010110;break;} // 38000001 li 0, 1
case 0x80010110:{uint32_t v;if(!word((r[2]+uint32_t(-32128)),v))return {};f[2]=bits(double(single(v)));c.paired[2]=v;c.pc=0x80010114;break;} // c0428280 lfs 2, -32128(2)
case 0x80010114:{if(!store((r[31]+uint32_t(412)),r[0]))return {};c.pc=0x80010118;break;} // 901f019c stw 0, 412(31)
case 0x80010118:{r[3]=r[31]+uint32_t(12);c.pc=0x8001011c;break;} // 387f000c addi 3, 31, 12
case 0x8001011c:{uint32_t v;if(!word((r[2]+uint32_t(-32112)),v))return {};f[1]=bits(double(single(v)));c.paired[1]=v;c.pc=0x80010120;break;} // c0228290 lfs 1, -32112(2)
case 0x80010120:{r[4]=r[31]+uint32_t(24);c.pc=0x80010124;break;} // 389f0018 addi 4, 31, 24
case 0x80010124:{if(!word((r[13]+uint32_t(-32624)),r[5]))return {};c.pc=0x80010128;break;} // 80ad8090 lwz 5, -32624(13)
case 0x80010128:{if(!word((r[13]+uint32_t(-32620)),r[0]))return {};c.pc=0x8001012c;break;} // 800d8094 lwz 0, -32620(13)
case 0x8001012c:{uint32_t v;if(!word((r[2]+uint32_t(-32140)),v))return {};f[0]=bits(double(single(v)));c.paired[0]=v;c.pc=0x80010130;break;} // c0028274 lfs 0, -32140(2)
case 0x80010130:{if(!store((r[31]+uint32_t(12)),r[5]))return {};c.pc=0x80010134;break;} // 90bf000c stw 5, 12(31)
case 0x80010134:{if(!store((r[31]+uint32_t(16)),r[0]))return {};c.pc=0x80010138;break;} // 901f0010 stw 0, 16(31)
case 0x80010138:{if(!word((r[13]+uint32_t(-32616)),r[0]))return {};c.pc=0x8001013c;break;} // 800d8098 lwz 0, -32616(13)
case 0x8001013c:{if(!store((r[31]+uint32_t(20)),r[0]))return {};c.pc=0x80010140;break;} // 901f0014 stw 0, 20(31)
case 0x80010140:{if(!word((r[13]+uint32_t(-32612)),r[5]))return {};c.pc=0x80010144;break;} // 80ad809c lwz 5, -32612(13)
case 0x80010144:{if(!word((r[13]+uint32_t(-32608)),r[0]))return {};c.pc=0x80010148;break;} // 800d80a0 lwz 0, -32608(13)
case 0x80010148:{if(!store((r[31]+uint32_t(24)),r[5]))return {};c.pc=0x8001014c;break;} // 90bf0018 stw 5, 24(31)
case 0x8001014c:{if(!store((r[31]+uint32_t(28)),r[0]))return {};c.pc=0x80010150;break;} // 901f001c stw 0, 28(31)
case 0x80010150:{if(!word((r[13]+uint32_t(-32604)),r[0]))return {};c.pc=0x80010154;break;} // 800d80a4 lwz 0, -32604(13)
case 0x80010154:{if(!store((r[31]+uint32_t(32)),r[0]))return {};c.pc=0x80010158;break;} // 901f0020 stw 0, 32(31)
case 0x80010158:{if(!word((r[13]+uint32_t(-32600)),r[5]))return {};c.pc=0x8001015c;break;} // 80ad80a8 lwz 5, -32600(13)
case 0x8001015c:{if(!word((r[13]+uint32_t(-32596)),r[0]))return {};c.pc=0x80010160;break;} // 800d80ac lwz 0, -32596(13)
case 0x80010160:{if(!store((r[31]+uint32_t(36)),r[5]))return {};c.pc=0x80010164;break;} // 90bf0024 stw 5, 36(31)
case 0x80010164:{if(!store((r[31]+uint32_t(40)),r[0]))return {};c.pc=0x80010168;break;} // 901f0028 stw 0, 40(31)
case 0x80010168:{if(!word((r[13]+uint32_t(-32592)),r[0]))return {};c.pc=0x8001016c;break;} // 800d80b0 lwz 0, -32592(13)
case 0x8001016c:{if(!store((r[31]+uint32_t(44)),r[0]))return {};c.pc=0x80010170;break;} // 901f002c stw 0, 44(31)
case 0x80010170:{if(!store((r[31]+uint32_t(48)),singleBits(scalar(f[2]))))return {};c.pc=0x80010174;break;} // d05f0030 stfs 2, 48(31)
case 0x80010174:{if(!store((r[31]+uint32_t(52)),singleBits(scalar(f[1]))))return {};c.pc=0x80010178;break;} // d03f0034 stfs 1, 52(31)
case 0x80010178:{if(!store((r[31]+uint32_t(276)),singleBits(scalar(f[0]))))return {};c.pc=0x8001017c;break;} // d01f0114 stfs 0, 276(31)
case 0x8001017c:{if(!external(0x8029eeb4,0x80010180))return {};break;} // 4828ed39 bl 0x8029eeb4
case 0x80010180:{if(!store((r[31]+uint32_t(280)),singleBits(scalar(f[1]))))return {};c.pc=0x80010184;break;} // d03f0118 stfs 1, 280(31)
case 0x80010184:{r[5]=uint32_t(0);c.pc=0x80010188;break;} // 38a00000 li 5, 0
case 0x80010188:{uint32_t v;if(!word((r[2]+uint32_t(-32140)),v))return {};f[2]=bits(double(single(v)));c.paired[2]=v;c.pc=0x8001018c;break;} // c0428274 lfs 2, -32140(2)
case 0x8001018c:{r[0]=uint32_t(8);c.pc=0x80010190;break;} // 38000008 li 0, 8
case 0x80010190:{if(!store((r[31]+uint32_t(492)),r[5]))return {};c.pc=0x80010194;break;} // 90bf01ec stw 5, 492(31)
case 0x80010194:{r[3]=r[31]+uint32_t(596);c.pc=0x80010198;break;} // 387f0254 addi 3, 31, 596
case 0x80010198:{uint32_t v;if(!word((r[2]+uint32_t(-32104)),v))return {};f[1]=bits(double(single(v)));c.paired[1]=v;c.pc=0x8001019c;break;} // c0228298 lfs 1, -32104(2)
case 0x8001019c:{r[4]=r[30]+uint32_t(112);c.pc=0x800101a0;break;} // 389e0070 addi 4, 30, 112
case 0x800101a0:{if(!s.storeHalf||!s.storeHalf(s.user,(r[31]+uint32_t(2)),uint16_t(r[5])))return {};c.pc=0x800101a4;break;} // b0bf0002 sth 5, 2(31)
case 0x800101a4:{uint32_t v;if(!word((r[2]+uint32_t(-32116)),v))return {};f[0]=bits(double(single(v)));c.paired[0]=v;c.pc=0x800101a8;break;} // c002828c lfs 0, -32116(2)
case 0x800101a8:{if(!store((r[31]+uint32_t(332)),singleBits(scalar(f[2]))))return {};c.pc=0x800101ac;break;} // d05f014c stfs 2, 332(31)
case 0x800101ac:{if(!store((r[31]+uint32_t(344)),singleBits(scalar(f[2]))))return {};c.pc=0x800101b0;break;} // d05f0158 stfs 2, 344(31)
case 0x800101b0:{if(!store((r[31]+uint32_t(340)),singleBits(scalar(f[2]))))return {};c.pc=0x800101b4;break;} // d05f0154 stfs 2, 340(31)
case 0x800101b4:{if(!store((r[31]+uint32_t(336)),singleBits(scalar(f[2]))))return {};c.pc=0x800101b8;break;} // d05f0150 stfs 2, 336(31)
case 0x800101b8:{if(!store((r[31]+uint32_t(60)),singleBits(scalar(f[1]))))return {};c.pc=0x800101bc;break;} // d03f003c stfs 1, 60(31)
case 0x800101bc:{if(!store((r[31]+uint32_t(56)),singleBits(scalar(f[0]))))return {};c.pc=0x800101c0;break;} // d01f0038 stfs 0, 56(31)
case 0x800101c0:{if(!store((r[31]+uint32_t(488)),r[0]))return {};c.pc=0x800101c4;break;} // 901f01e8 stw 0, 488(31)
case 0x800101c4:{if(!external(0x8026d5fc,0x800101c8))return {};break;} // 4825d439 bl 0x8026d5fc
case 0x800101c8:{r[0]=uint32_t(0);c.pc=0x800101cc;break;} // 38000000 li 0, 0
case 0x800101cc:{if(!s.storeHalf||!s.storeHalf(s.user,(r[31]+uint32_t(244)),uint16_t(r[0])))return {};c.pc=0x800101d0;break;} // b01f00f4 sth 0, 244(31)
case 0x800101d0:{if(!s.storeHalf||!s.storeHalf(s.user,(r[31]+uint32_t(246)),uint16_t(r[0])))return {};c.pc=0x800101d4;break;} // b01f00f6 sth 0, 246(31)
case 0x800101d4:{c.lr=0x800101d8;c.pc=0x8002eb30;break;} // 4801e95d bl 0x8002eb30
case 0x800101d8:{if(!half((r[3]+uint32_t(4)),r[0]))return {};c.pc=0x800101dc;break;} // a0030004 lhz 0, 4(3)
case 0x800101dc:{if(!s.storeHalf||!s.storeHalf(s.user,(r[31]+uint32_t(248)),uint16_t(r[0])))return {};c.pc=0x800101e0;break;} // b01f00f8 sth 0, 248(31)
case 0x800101e0:{c.lr=0x800101e4;c.pc=0x8002eb30;break;} // 4801e951 bl 0x8002eb30
case 0x800101e4:{if(!half((r[3]+uint32_t(6)),r[0]))return {};c.pc=0x800101e8;break;} // a0030006 lhz 0, 6(3)
case 0x800101e8:{uint32_t v;if(!word((r[2]+uint32_t(-32140)),v))return {};f[0]=bits(double(single(v)));c.paired[0]=v;c.pc=0x800101ec;break;} // c0028274 lfs 0, -32140(2)
case 0x800101ec:{if(!s.storeHalf||!s.storeHalf(s.user,(r[31]+uint32_t(250)),uint16_t(r[0])))return {};c.pc=0x800101f0;break;} // b01f00fa sth 0, 250(31)
case 0x800101f0:{if(!store((r[31]+uint32_t(252)),singleBits(scalar(f[0]))))return {};c.pc=0x800101f4;break;} // d01f00fc stfs 0, 252(31)
case 0x800101f4:{if(!store((r[31]+uint32_t(256)),singleBits(scalar(f[0]))))return {};c.pc=0x800101f8;break;} // d01f0100 stfs 0, 256(31)
case 0x800101f8:{c.lr=0x800101fc;c.pc=0x8002eb30;break;} // 4801e939 bl 0x8002eb30
case 0x800101fc:{if(!half((r[3]+uint32_t(4)),r[3]))return {};c.pc=0x80010200;break;} // a0630004 lhz 3, 4(3)
case 0x80010200:{r[0]=uint32_t(17200)<<16;c.pc=0x80010204;break;} // 3c004330 lis 0, 17200
case 0x80010204:{if(!store((r[1]+uint32_t(88)),r[0]))return {};c.pc=0x80010208;break;} // 90010058 stw 0, 88(1)
case 0x80010208:{if(!c.step){if(!word((r[30]+uint32_t(104)),c.transfer))return {};c.step=1;}uint32_t low;if(!word((r[30]+uint32_t(104))+4,low))return {};f[1]=(uint64_t(c.transfer)<<32)|low;c.step=0;c.pc=0x8001020c;break;} // c83e0068 lfd 1, 104(30)
case 0x8001020c:{if(!store((r[1]+uint32_t(92)),r[3]))return {};c.pc=0x80010210;break;} // 9061005c stw 3, 92(1)
case 0x80010210:{if(!c.step){if(!word((r[1]+uint32_t(88)),c.transfer))return {};c.step=1;}uint32_t low;if(!word((r[1]+uint32_t(88))+4,low))return {};f[0]=(uint64_t(c.transfer)<<32)|low;c.step=0;c.pc=0x80010214;break;} // c8010058 lfd 0, 88(1)
case 0x80010214:{f[0]=rounded(scalar(f[0])-scalar(f[1]));c.pc=0x80010218;break;} // ec000828 fsubs 0, 0, 1
case 0x80010218:{if(!store((r[31]+uint32_t(260)),singleBits(scalar(f[0]))))return {};c.pc=0x8001021c;break;} // d01f0104 stfs 0, 260(31)
case 0x8001021c:{c.lr=0x80010220;c.pc=0x8002eb30;break;} // 4801e915 bl 0x8002eb30
case 0x80010220:{if(!half((r[3]+uint32_t(6)),r[4]))return {};c.pc=0x80010224;break;} // a0830006 lhz 4, 6(3)
case 0x80010224:{r[0]=uint32_t(17200)<<16;c.pc=0x80010228;break;} // 3c004330 lis 0, 17200
case 0x80010228:{r[3]=uint32_t(-32707)<<16;c.pc=0x8001022c;break;} // 3c60803d lis 3, -32707
case 0x8001022c:{if(!store((r[1]+uint32_t(96)),r[0]))return {};c.pc=0x80010230;break;} // 90010060 stw 0, 96(1)
case 0x80010230:{if(!c.step){if(!word((r[30]+uint32_t(104)),c.transfer))return {};c.step=1;}uint32_t low;if(!word((r[30]+uint32_t(104))+4,low))return {};f[3]=(uint64_t(c.transfer)<<32)|low;c.step=0;c.pc=0x80010234;break;} // c87e0068 lfd 3, 104(30)
case 0x80010234:{r[5]=r[3]+uint32_t(-27144);c.pc=0x80010238;break;} // 38a395f8 addi 5, 3, -27144
case 0x80010238:{if(!store((r[1]+uint32_t(100)),r[4]))return {};c.pc=0x8001023c;break;} // 90810064 stw 4, 100(1)
case 0x8001023c:{r[3]=uint32_t(0);c.pc=0x80010240;break;} // 38600000 li 3, 0
case 0x80010240:{uint32_t v;if(!word((r[2]+uint32_t(-32140)),v))return {};f[1]=bits(double(single(v)));c.paired[1]=v;c.pc=0x80010244;break;} // c0228274 lfs 1, -32140(2)
case 0x80010244:{r[4]=uint32_t(608);c.pc=0x80010248;break;} // 38800260 li 4, 608
case 0x80010248:{if(!c.step){if(!word((r[1]+uint32_t(96)),c.transfer))return {};c.step=1;}uint32_t low;if(!word((r[1]+uint32_t(96))+4,low))return {};f[2]=(uint64_t(c.transfer)<<32)|low;c.step=0;c.pc=0x8001024c;break;} // c8410060 lfd 2, 96(1)
case 0x8001024c:{uint32_t v;if(!word((r[2]+uint32_t(-32128)),v))return {};f[0]=bits(double(single(v)));c.paired[0]=v;c.pc=0x80010250;break;} // c0028280 lfs 0, -32128(2)
case 0x80010250:{f[2]=rounded(scalar(f[2])-scalar(f[3]));c.pc=0x80010254;break;} // ec421828 fsubs 2, 2, 3
case 0x80010254:{if(!store((r[31]+uint32_t(264)),singleBits(scalar(f[2]))))return {};c.pc=0x80010258;break;} // d05f0108 stfs 2, 264(31)
case 0x80010258:{if(!store((r[31]+uint32_t(268)),singleBits(scalar(f[1]))))return {};c.pc=0x8001025c;break;} // d03f010c stfs 1, 268(31)
case 0x8001025c:{if(!store((r[31]+uint32_t(272)),singleBits(scalar(f[0]))))return {};c.pc=0x80010260;break;} // d01f0110 stfs 0, 272(31)
case 0x80010260:{if(!store((r[5]+uint32_t(8)),r[31]))return {};c.pc=0x80010264;break;} // 93e50008 stw 31, 8(5)
case 0x80010264:{if(!external(0x800300f0,0x80010268))return {};break;} // 4801fe8d bl 0x800300f0
case 0x80010268:{r[31]=r[3];c.pc=0x8001026c;break;} // 7c7f1b78 mr 31, 3
case 0x8001026c:{r[4]=uint32_t(0);c.pc=0x80010270;break;} // 38800000 li 4, 0
case 0x80010270:{r[5]=uint32_t(608);c.pc=0x80010274;break;} // 38a00260 li 5, 608
case 0x80010274:{if(!external(0x800050b4,0x80010278))return {};break;} // 4bff4e41 bl 0x800050b4
case 0x80010278:{uint32_t v;if(!word((r[2]+uint32_t(-32140)),v))return {};f[1]=bits(double(single(v)));c.paired[1]=v;c.pc=0x8001027c;break;} // c0228274 lfs 1, -32140(2)
case 0x8001027c:{r[3]=r[31]+uint32_t(348);c.pc=0x80010280;break;} // 387f015c addi 3, 31, 348
case 0x80010280:{uint32_t v;if(!word((r[2]+uint32_t(-32096)),v))return {};f[2]=bits(double(single(v)));c.paired[2]=v;c.pc=0x80010284;break;} // c04282a0 lfs 2, -32096(2)
case 0x80010284:{f[3]=f[1];c.pc=0x80010288;break;} // fc600890 fmr 3, 1
case 0x80010288:{uint32_t v;if(!word((r[2]+uint32_t(-32100)),v))return {};f[4]=bits(double(single(v)));c.paired[4]=v;c.pc=0x8001028c;break;} // c082829c lfs 4, -32100(2)
case 0x8001028c:{uint32_t v;if(!word((r[2]+uint32_t(-32128)),v))return {};f[5]=bits(double(single(v)));c.paired[5]=v;c.pc=0x80010290;break;} // c0a28280 lfs 5, -32128(2)
case 0x80010290:{uint32_t v;if(!word((r[2]+uint32_t(-32112)),v))return {};f[6]=bits(double(single(v)));c.paired[6]=v;c.pc=0x80010294;break;} // c0c28290 lfs 6, -32112(2)
case 0x80010294:{c.lr=0x80010298;c.pc=0x8029e8b8;break;} // 4828e625 bl 0x8029e8b8
case 0x80010298:{r[0]=uint32_t(1);c.pc=0x8001029c;break;} // 38000001 li 0, 1
case 0x8001029c:{uint32_t v;if(!word((r[2]+uint32_t(-32128)),v))return {};f[2]=bits(double(single(v)));c.paired[2]=v;c.pc=0x800102a0;break;} // c0428280 lfs 2, -32128(2)
case 0x800102a0:{if(!store((r[31]+uint32_t(412)),r[0]))return {};c.pc=0x800102a4;break;} // 901f019c stw 0, 412(31)
case 0x800102a4:{r[3]=r[31]+uint32_t(12);c.pc=0x800102a8;break;} // 387f000c addi 3, 31, 12
case 0x800102a8:{uint32_t v;if(!word((r[2]+uint32_t(-32112)),v))return {};f[1]=bits(double(single(v)));c.paired[1]=v;c.pc=0x800102ac;break;} // c0228290 lfs 1, -32112(2)
case 0x800102ac:{r[4]=r[31]+uint32_t(24);c.pc=0x800102b0;break;} // 389f0018 addi 4, 31, 24
case 0x800102b0:{if(!word((r[13]+uint32_t(-32624)),r[5]))return {};c.pc=0x800102b4;break;} // 80ad8090 lwz 5, -32624(13)
case 0x800102b4:{if(!word((r[13]+uint32_t(-32620)),r[0]))return {};c.pc=0x800102b8;break;} // 800d8094 lwz 0, -32620(13)
case 0x800102b8:{uint32_t v;if(!word((r[2]+uint32_t(-32140)),v))return {};f[0]=bits(double(single(v)));c.paired[0]=v;c.pc=0x800102bc;break;} // c0028274 lfs 0, -32140(2)
case 0x800102bc:{if(!store((r[31]+uint32_t(12)),r[5]))return {};c.pc=0x800102c0;break;} // 90bf000c stw 5, 12(31)
case 0x800102c0:{if(!store((r[31]+uint32_t(16)),r[0]))return {};c.pc=0x800102c4;break;} // 901f0010 stw 0, 16(31)
case 0x800102c4:{if(!word((r[13]+uint32_t(-32616)),r[0]))return {};c.pc=0x800102c8;break;} // 800d8098 lwz 0, -32616(13)
case 0x800102c8:{if(!store((r[31]+uint32_t(20)),r[0]))return {};c.pc=0x800102cc;break;} // 901f0014 stw 0, 20(31)
case 0x800102cc:{if(!word((r[13]+uint32_t(-32612)),r[5]))return {};c.pc=0x800102d0;break;} // 80ad809c lwz 5, -32612(13)
case 0x800102d0:{if(!word((r[13]+uint32_t(-32608)),r[0]))return {};c.pc=0x800102d4;break;} // 800d80a0 lwz 0, -32608(13)
case 0x800102d4:{if(!store((r[31]+uint32_t(24)),r[5]))return {};c.pc=0x800102d8;break;} // 90bf0018 stw 5, 24(31)
case 0x800102d8:{if(!store((r[31]+uint32_t(28)),r[0]))return {};c.pc=0x800102dc;break;} // 901f001c stw 0, 28(31)
case 0x800102dc:{if(!word((r[13]+uint32_t(-32604)),r[0]))return {};c.pc=0x800102e0;break;} // 800d80a4 lwz 0, -32604(13)
case 0x800102e0:{if(!store((r[31]+uint32_t(32)),r[0]))return {};c.pc=0x800102e4;break;} // 901f0020 stw 0, 32(31)
case 0x800102e4:{if(!word((r[13]+uint32_t(-32600)),r[5]))return {};c.pc=0x800102e8;break;} // 80ad80a8 lwz 5, -32600(13)
case 0x800102e8:{if(!word((r[13]+uint32_t(-32596)),r[0]))return {};c.pc=0x800102ec;break;} // 800d80ac lwz 0, -32596(13)
case 0x800102ec:{if(!store((r[31]+uint32_t(36)),r[5]))return {};c.pc=0x800102f0;break;} // 90bf0024 stw 5, 36(31)
case 0x800102f0:{if(!store((r[31]+uint32_t(40)),r[0]))return {};c.pc=0x800102f4;break;} // 901f0028 stw 0, 40(31)
case 0x800102f4:{if(!word((r[13]+uint32_t(-32592)),r[0]))return {};c.pc=0x800102f8;break;} // 800d80b0 lwz 0, -32592(13)
case 0x800102f8:{if(!store((r[31]+uint32_t(44)),r[0]))return {};c.pc=0x800102fc;break;} // 901f002c stw 0, 44(31)
case 0x800102fc:{if(!store((r[31]+uint32_t(48)),singleBits(scalar(f[2]))))return {};c.pc=0x80010300;break;} // d05f0030 stfs 2, 48(31)
case 0x80010300:{if(!store((r[31]+uint32_t(52)),singleBits(scalar(f[1]))))return {};c.pc=0x80010304;break;} // d03f0034 stfs 1, 52(31)
case 0x80010304:{if(!store((r[31]+uint32_t(276)),singleBits(scalar(f[0]))))return {};c.pc=0x80010308;break;} // d01f0114 stfs 0, 276(31)
case 0x80010308:{if(!external(0x8029eeb4,0x8001030c))return {};break;} // 4828ebad bl 0x8029eeb4
case 0x8001030c:{if(!store((r[31]+uint32_t(280)),singleBits(scalar(f[1]))))return {};c.pc=0x80010310;break;} // d03f0118 stfs 1, 280(31)
case 0x80010310:{r[5]=uint32_t(0);c.pc=0x80010314;break;} // 38a00000 li 5, 0
case 0x80010314:{uint32_t v;if(!word((r[2]+uint32_t(-32140)),v))return {};f[2]=bits(double(single(v)));c.paired[2]=v;c.pc=0x80010318;break;} // c0428274 lfs 2, -32140(2)
case 0x80010318:{r[0]=uint32_t(8);c.pc=0x8001031c;break;} // 38000008 li 0, 8
case 0x8001031c:{if(!store((r[31]+uint32_t(492)),r[5]))return {};c.pc=0x80010320;break;} // 90bf01ec stw 5, 492(31)
case 0x80010320:{r[3]=r[31]+uint32_t(596);c.pc=0x80010324;break;} // 387f0254 addi 3, 31, 596
case 0x80010324:{uint32_t v;if(!word((r[2]+uint32_t(-32104)),v))return {};f[1]=bits(double(single(v)));c.paired[1]=v;c.pc=0x80010328;break;} // c0228298 lfs 1, -32104(2)
case 0x80010328:{r[4]=r[2]+uint32_t(-32040);c.pc=0x8001032c;break;} // 388282d8 addi 4, 2, -32040
case 0x8001032c:{if(!s.storeHalf||!s.storeHalf(s.user,(r[31]+uint32_t(2)),uint16_t(r[5])))return {};c.pc=0x80010330;break;} // b0bf0002 sth 5, 2(31)
case 0x80010330:{uint32_t v;if(!word((r[2]+uint32_t(-32116)),v))return {};f[0]=bits(double(single(v)));c.paired[0]=v;c.pc=0x80010334;break;} // c002828c lfs 0, -32116(2)
case 0x80010334:{if(!store((r[31]+uint32_t(332)),singleBits(scalar(f[2]))))return {};c.pc=0x80010338;break;} // d05f014c stfs 2, 332(31)
case 0x80010338:{if(!store((r[31]+uint32_t(344)),singleBits(scalar(f[2]))))return {};c.pc=0x8001033c;break;} // d05f0158 stfs 2, 344(31)
case 0x8001033c:{if(!store((r[31]+uint32_t(340)),singleBits(scalar(f[2]))))return {};c.pc=0x80010340;break;} // d05f0154 stfs 2, 340(31)
case 0x80010340:{if(!store((r[31]+uint32_t(336)),singleBits(scalar(f[2]))))return {};c.pc=0x80010344;break;} // d05f0150 stfs 2, 336(31)
case 0x80010344:{if(!store((r[31]+uint32_t(60)),singleBits(scalar(f[1]))))return {};c.pc=0x80010348;break;} // d03f003c stfs 1, 60(31)
case 0x80010348:{if(!store((r[31]+uint32_t(56)),singleBits(scalar(f[0]))))return {};c.pc=0x8001034c;break;} // d01f0038 stfs 0, 56(31)
case 0x8001034c:{if(!store((r[31]+uint32_t(488)),r[0]))return {};c.pc=0x80010350;break;} // 901f01e8 stw 0, 488(31)
case 0x80010350:{if(!external(0x8026d5fc,0x80010354))return {};break;} // 4825d2ad bl 0x8026d5fc
case 0x80010354:{r[0]=uint32_t(0);c.pc=0x80010358;break;} // 38000000 li 0, 0
case 0x80010358:{if(!s.storeHalf||!s.storeHalf(s.user,(r[31]+uint32_t(244)),uint16_t(r[0])))return {};c.pc=0x8001035c;break;} // b01f00f4 sth 0, 244(31)
case 0x8001035c:{if(!s.storeHalf||!s.storeHalf(s.user,(r[31]+uint32_t(246)),uint16_t(r[0])))return {};c.pc=0x80010360;break;} // b01f00f6 sth 0, 246(31)
case 0x80010360:{c.lr=0x80010364;c.pc=0x8002eb30;break;} // 4801e7d1 bl 0x8002eb30
case 0x80010364:{if(!half((r[3]+uint32_t(4)),r[0]))return {};c.pc=0x80010368;break;} // a0030004 lhz 0, 4(3)
case 0x80010368:{if(!s.storeHalf||!s.storeHalf(s.user,(r[31]+uint32_t(248)),uint16_t(r[0])))return {};c.pc=0x8001036c;break;} // b01f00f8 sth 0, 248(31)
case 0x8001036c:{c.lr=0x80010370;c.pc=0x8002eb30;break;} // 4801e7c5 bl 0x8002eb30
case 0x80010370:{if(!half((r[3]+uint32_t(6)),r[0]))return {};c.pc=0x80010374;break;} // a0030006 lhz 0, 6(3)
case 0x80010374:{uint32_t v;if(!word((r[2]+uint32_t(-32140)),v))return {};f[0]=bits(double(single(v)));c.paired[0]=v;c.pc=0x80010378;break;} // c0028274 lfs 0, -32140(2)
case 0x80010378:{if(!s.storeHalf||!s.storeHalf(s.user,(r[31]+uint32_t(250)),uint16_t(r[0])))return {};c.pc=0x8001037c;break;} // b01f00fa sth 0, 250(31)
case 0x8001037c:{if(!store((r[31]+uint32_t(252)),singleBits(scalar(f[0]))))return {};c.pc=0x80010380;break;} // d01f00fc stfs 0, 252(31)
case 0x80010380:{if(!store((r[31]+uint32_t(256)),singleBits(scalar(f[0]))))return {};c.pc=0x80010384;break;} // d01f0100 stfs 0, 256(31)
case 0x80010384:{c.lr=0x80010388;c.pc=0x8002eb30;break;} // 4801e7ad bl 0x8002eb30
case 0x80010388:{if(!half((r[3]+uint32_t(4)),r[3]))return {};c.pc=0x8001038c;break;} // a0630004 lhz 3, 4(3)
case 0x8001038c:{r[0]=uint32_t(17200)<<16;c.pc=0x80010390;break;} // 3c004330 lis 0, 17200
case 0x80010390:{if(!store((r[1]+uint32_t(104)),r[0]))return {};c.pc=0x80010394;break;} // 90010068 stw 0, 104(1)
case 0x80010394:{if(!c.step){if(!word((r[30]+uint32_t(104)),c.transfer))return {};c.step=1;}uint32_t low;if(!word((r[30]+uint32_t(104))+4,low))return {};f[1]=(uint64_t(c.transfer)<<32)|low;c.step=0;c.pc=0x80010398;break;} // c83e0068 lfd 1, 104(30)
case 0x80010398:{if(!store((r[1]+uint32_t(108)),r[3]))return {};c.pc=0x8001039c;break;} // 9061006c stw 3, 108(1)
case 0x8001039c:{if(!c.step){if(!word((r[1]+uint32_t(104)),c.transfer))return {};c.step=1;}uint32_t low;if(!word((r[1]+uint32_t(104))+4,low))return {};f[0]=(uint64_t(c.transfer)<<32)|low;c.step=0;c.pc=0x800103a0;break;} // c8010068 lfd 0, 104(1)
case 0x800103a0:{f[0]=rounded(scalar(f[0])-scalar(f[1]));c.pc=0x800103a4;break;} // ec000828 fsubs 0, 0, 1
case 0x800103a4:{if(!store((r[31]+uint32_t(260)),singleBits(scalar(f[0]))))return {};c.pc=0x800103a8;break;} // d01f0104 stfs 0, 260(31)
case 0x800103a8:{c.lr=0x800103ac;c.pc=0x8002eb30;break;} // 4801e789 bl 0x8002eb30
case 0x800103ac:{if(!half((r[3]+uint32_t(6)),r[3]))return {};c.pc=0x800103b0;break;} // a0630006 lhz 3, 6(3)
case 0x800103b0:{r[0]=uint32_t(17200)<<16;c.pc=0x800103b4;break;} // 3c004330 lis 0, 17200
case 0x800103b4:{uint32_t v;if(!word((r[2]+uint32_t(-32128)),v))return {};f[3]=bits(double(single(v)));c.paired[3]=v;c.pc=0x800103b8;break;} // c0628280 lfs 3, -32128(2)
case 0x800103b8:{r[4]=uint32_t(-32707)<<16;c.pc=0x800103bc;break;} // 3c80803d lis 4, -32707
case 0x800103bc:{if(!store((r[1]+uint32_t(116)),r[3]))return {};c.pc=0x800103c0;break;} // 90610074 stw 3, 116(1)
case 0x800103c0:{r[5]=r[4]+uint32_t(-27144);c.pc=0x800103c4;break;} // 38a495f8 addi 5, 4, -27144
case 0x800103c4:{r[3]=uint32_t(-32767)<<16;c.pc=0x800103c8;break;} // 3c608001 lis 3, -32767
case 0x800103c8:{if(!c.step){if(!word((r[30]+uint32_t(104)),c.transfer))return {};c.step=1;}uint32_t low;if(!word((r[30]+uint32_t(104))+4,low))return {};f[1]=(uint64_t(c.transfer)<<32)|low;c.step=0;c.pc=0x800103cc;break;} // c83e0068 lfd 1, 104(30)
case 0x800103cc:{if(!store((r[1]+uint32_t(112)),r[0]))return {};c.pc=0x800103d0;break;} // 90010070 stw 0, 112(1)
case 0x800103d0:{f[6]=f[3];c.pc=0x800103d4;break;} // fcc01890 fmr 6, 3
case 0x800103d4:{uint32_t v;if(!word((r[2]+uint32_t(-32140)),v))return {};f[5]=bits(double(single(v)));c.paired[5]=v;c.pc=0x800103d8;break;} // c0a28274 lfs 5, -32140(2)
case 0x800103d8:{r[3]=r[3]+uint32_t(-5528);c.pc=0x800103dc;break;} // 3863ea68 addi 3, 3, -5528
case 0x800103dc:{if(!c.step){if(!word((r[1]+uint32_t(112)),c.transfer))return {};c.step=1;}uint32_t low;if(!word((r[1]+uint32_t(112))+4,low))return {};f[0]=(uint64_t(c.transfer)<<32)|low;c.step=0;c.pc=0x800103e0;break;} // c8010070 lfd 0, 112(1)
case 0x800103e0:{r[4]=r[2]+uint32_t(-32036);c.pc=0x800103e4;break;} // 388282dc addi 4, 2, -32036
case 0x800103e4:{uint32_t v;if(!word((r[2]+uint32_t(-32104)),v))return {};f[2]=bits(double(single(v)));c.paired[2]=v;c.pc=0x800103e8;break;} // c0428298 lfs 2, -32104(2)
case 0x800103e8:{f[0]=rounded(scalar(f[0])-scalar(f[1]));c.pc=0x800103ec;break;} // ec000828 fsubs 0, 0, 1
case 0x800103ec:{uint32_t v;if(!word((r[2]+uint32_t(-32116)),v))return {};f[1]=bits(double(single(v)));c.paired[1]=v;c.pc=0x800103f0;break;} // c022828c lfs 1, -32116(2)
case 0x800103f0:{uint32_t v;if(!word((r[2]+uint32_t(-32112)),v))return {};f[4]=bits(double(single(v)));c.paired[4]=v;c.pc=0x800103f4;break;} // c0828290 lfs 4, -32112(2)
case 0x800103f4:{if(!store((r[31]+uint32_t(264)),singleBits(scalar(f[0]))))return {};c.pc=0x800103f8;break;} // d01f0108 stfs 0, 264(31)
case 0x800103f8:{if(!store((r[31]+uint32_t(268)),singleBits(scalar(f[5]))))return {};c.pc=0x800103fc;break;} // d0bf010c stfs 5, 268(31)
case 0x800103fc:{if(!store((r[31]+uint32_t(272)),singleBits(scalar(f[3]))))return {};c.pc=0x80010400;break;} // d07f0110 stfs 3, 272(31)
case 0x80010400:{if(!store((r[5]+uint32_t(12)),r[31]))return {};c.pc=0x80010404;break;} // 93e5000c stw 31, 12(5)
case 0x80010404:{c.lr=0x80010408;c.pc=0x8000fac8;break;} // 4bfff6c5 bl 0x8000fac8
case 0x80010408:{r[5]=uint32_t(-32707)<<16;c.pc=0x8001040c;break;} // 3ca0803d lis 5, -32707
case 0x8001040c:{r[4]=uint32_t(-32767)<<16;c.pc=0x80010410;break;} // 3c808001 lis 4, -32767
case 0x80010410:{r[5]=r[5]+uint32_t(-27144);c.pc=0x80010414;break;} // 38a595f8 addi 5, 5, -27144
case 0x80010414:{uint32_t v;if(!word((r[2]+uint32_t(-32116)),v))return {};f[1]=bits(double(single(v)));c.paired[1]=v;c.pc=0x80010418;break;} // c022828c lfs 1, -32116(2)
case 0x80010418:{if(!store((r[5]+uint32_t(16)),r[3]))return {};c.pc=0x8001041c;break;} // 90650010 stw 3, 16(5)
case 0x8001041c:{r[3]=r[4]+uint32_t(-5772);c.pc=0x80010420;break;} // 3864e974 addi 3, 4, -5772
case 0x80010420:{uint32_t v;if(!word((r[2]+uint32_t(-32104)),v))return {};f[2]=bits(double(single(v)));c.paired[2]=v;c.pc=0x80010424;break;} // c0428298 lfs 2, -32104(2)
case 0x80010424:{r[4]=r[30]+uint32_t(120);c.pc=0x80010428;break;} // 389e0078 addi 4, 30, 120
case 0x80010428:{uint32_t v;if(!word((r[2]+uint32_t(-32108)),v))return {};f[3]=bits(double(single(v)));c.paired[3]=v;c.pc=0x8001042c;break;} // c0628294 lfs 3, -32108(2)
case 0x8001042c:{uint32_t v;if(!word((r[2]+uint32_t(-32032)),v))return {};f[4]=bits(double(single(v)));c.paired[4]=v;c.pc=0x80010430;break;} // c08282e0 lfs 4, -32032(2)
case 0x80010430:{uint32_t v;if(!word((r[2]+uint32_t(-32072)),v))return {};f[5]=bits(double(single(v)));c.paired[5]=v;c.pc=0x80010434;break;} // c0a282b8 lfs 5, -32072(2)
case 0x80010434:{uint32_t v;if(!word((r[2]+uint32_t(-32028)),v))return {};f[6]=bits(double(single(v)));c.paired[6]=v;c.pc=0x80010438;break;} // c0c282e4 lfs 6, -32028(2)
case 0x80010438:{c.lr=0x8001043c;c.pc=0x8000fac8;break;} // 4bfff691 bl 0x8000fac8
case 0x8001043c:{uint32_t v;if(!word((r[2]+uint32_t(-32128)),v))return {};f[3]=bits(double(single(v)));c.paired[3]=v;c.pc=0x80010440;break;} // c0628280 lfs 3, -32128(2)
case 0x80010440:{r[4]=uint32_t(-32707)<<16;c.pc=0x80010444;break;} // 3c80803d lis 4, -32707
case 0x80010444:{r[5]=r[4]+uint32_t(-27144);c.pc=0x80010448;break;} // 38a495f8 addi 5, 4, -27144
case 0x80010448:{uint32_t v;if(!word((r[2]+uint32_t(-32116)),v))return {};f[1]=bits(double(single(v)));c.paired[1]=v;c.pc=0x8001044c;break;} // c022828c lfs 1, -32116(2)
case 0x8001044c:{f[6]=f[3];c.pc=0x80010450;break;} // fcc01890 fmr 6, 3
case 0x80010450:{r[4]=uint32_t(-32767)<<16;c.pc=0x80010454;break;} // 3c808001 lis 4, -32767
case 0x80010454:{if(!store((r[5]+uint32_t(20)),r[3]))return {};c.pc=0x80010458;break;} // 90650014 stw 3, 20(5)
case 0x80010458:{r[3]=r[4]+uint32_t(-5584);c.pc=0x8001045c;break;} // 3864ea30 addi 3, 4, -5584
case 0x8001045c:{uint32_t v;if(!word((r[2]+uint32_t(-32024)),v))return {};f[2]=bits(double(single(v)));c.paired[2]=v;c.pc=0x80010460;break;} // c04282e8 lfs 2, -32024(2)
case 0x80010460:{r[4]=r[2]+uint32_t(-32020);c.pc=0x80010464;break;} // 388282ec addi 4, 2, -32020
case 0x80010464:{uint32_t v;if(!word((r[2]+uint32_t(-32112)),v))return {};f[4]=bits(double(single(v)));c.paired[4]=v;c.pc=0x80010468;break;} // c0828290 lfs 4, -32112(2)
case 0x80010468:{uint32_t v;if(!word((r[2]+uint32_t(-32140)),v))return {};f[5]=bits(double(single(v)));c.paired[5]=v;c.pc=0x8001046c;break;} // c0a28274 lfs 5, -32140(2)
case 0x8001046c:{c.lr=0x80010470;c.pc=0x8000fac8;break;} // 4bfff65d bl 0x8000fac8
case 0x80010470:{r[4]=uint32_t(-32707)<<16;c.pc=0x80010474;break;} // 3c80803d lis 4, -32707
case 0x80010474:{if(!c.step){if(!word((r[30]+uint32_t(128)),c.transfer))return {};c.step=1;}uint32_t low;if(!word((r[30]+uint32_t(128))+4,low))return {};f[1]=(uint64_t(c.transfer)<<32)|low;c.step=0;c.pc=0x80010478;break;} // c83e0080 lfd 1, 128(30)
case 0x80010478:{r[31]=r[4]+uint32_t(-27144);c.pc=0x8001047c;break;} // 3be495f8 addi 31, 4, -27144
case 0x8001047c:{if(!store((r[31]+uint32_t(24)),r[3]))return {};c.pc=0x80010480;break;} // 907f0018 stw 3, 24(31)
case 0x80010480:{if(!external(0x802724e8,0x80010484))return {};break;} // 48262069 bl 0x802724e8
case 0x80010484:{f[1]=rounded(scalar(f[1]));c.pc=0x80010488;break;} // fc200818 frsp 1, 1
case 0x80010488:{uint32_t v;if(!word((r[2]+uint32_t(-32012)),v))return {};f[0]=bits(double(single(v)));c.paired[0]=v;c.pc=0x8001048c;break;} // c00282f4 lfs 0, -32012(2)
case 0x8001048c:{if(!word((r[30]+uint32_t(72)),r[4]))return {};c.pc=0x80010490;break;} // 809e0048 lwz 4, 72(30)
case 0x80010490:{r[3]=uint32_t(-32767)<<16;c.pc=0x80010494;break;} // 3c608001 lis 3, -32767
case 0x80010494:{if(!word((r[30]+uint32_t(76)),r[5]))return {};c.pc=0x80010498;break;} // 80be004c lwz 5, 76(30)
case 0x80010498:{r[3]=r[3]+uint32_t(-5772);c.pc=0x8001049c;break;} // 3863e974 addi 3, 3, -5772
case 0x8001049c:{f[0]=rounded(scalar(f[0])/scalar(f[1]));c.pc=0x800104a0;break;} // ec000824 fdivs 0, 0, 1
case 0x800104a0:{if(!store((r[1]+uint32_t(44)),r[4]))return {};c.pc=0x800104a4;break;} // 9081002c stw 4, 44(1)
case 0x800104a4:{uint32_t v;if(!word((r[2]+uint32_t(-32140)),v))return {};f[2]=bits(double(single(v)));c.paired[2]=v;c.pc=0x800104a8;break;} // c0428274 lfs 2, -32140(2)
case 0x800104a8:{r[4]=r[30]+uint32_t(136);c.pc=0x800104ac;break;} // 389e0088 addi 4, 30, 136
case 0x800104ac:{if(!word((r[30]+uint32_t(80)),r[0]))return {};c.pc=0x800104b0;break;} // 801e0050 lwz 0, 80(30)
case 0x800104b0:{if(!store((r[1]+uint32_t(48)),r[5]))return {};c.pc=0x800104b4;break;} // 90a10030 stw 5, 48(1)
case 0x800104b4:{if(!store((r[1]+uint32_t(52)),r[0]))return {};c.pc=0x800104b8;break;} // 90010034 stw 0, 52(1)
case 0x800104b8:{if(!word((r[31]+uint32_t(24)),r[6]))return {};c.pc=0x800104bc;break;} // 80df0018 lwz 6, 24(31)
case 0x800104bc:{if(!store((r[1]+uint32_t(44)),singleBits(scalar(f[2]))))return {};c.pc=0x800104c0;break;} // d041002c stfs 2, 44(1)
case 0x800104c0:{uint32_t v;if(!word((r[2]+uint32_t(-32116)),v))return {};f[1]=bits(double(single(v)));c.paired[1]=v;c.pc=0x800104c4;break;} // c022828c lfs 1, -32116(2)
case 0x800104c4:{if(!store((r[1]+uint32_t(48)),singleBits(scalar(f[2]))))return {};c.pc=0x800104c8;break;} // d0410030 stfs 2, 48(1)
case 0x800104c8:{if(!word((r[1]+uint32_t(44)),r[0]))return {};c.pc=0x800104cc;break;} // 8001002c lwz 0, 44(1)
case 0x800104cc:{if(!store((r[1]+uint32_t(52)),singleBits(scalar(f[0]))))return {};c.pc=0x800104d0;break;} // d0010034 stfs 0, 52(1)
case 0x800104d0:{if(!word((r[1]+uint32_t(48)),r[5]))return {};c.pc=0x800104d4;break;} // 80a10030 lwz 5, 48(1)
case 0x800104d4:{if(!store((r[6]+uint32_t(12)),r[0]))return {};c.pc=0x800104d8;break;} // 9006000c stw 0, 12(6)
case 0x800104d8:{if(!word((r[1]+uint32_t(52)),r[0]))return {};c.pc=0x800104dc;break;} // 80010034 lwz 0, 52(1)
case 0x800104dc:{if(!store((r[6]+uint32_t(16)),r[5]))return {};c.pc=0x800104e0;break;} // 90a60010 stw 5, 16(6)
case 0x800104e0:{uint32_t v;if(!word((r[2]+uint32_t(-32104)),v))return {};f[2]=bits(double(single(v)));c.paired[2]=v;c.pc=0x800104e4;break;} // c0428298 lfs 2, -32104(2)
case 0x800104e4:{uint32_t v;if(!word((r[2]+uint32_t(-32108)),v))return {};f[3]=bits(double(single(v)));c.paired[3]=v;c.pc=0x800104e8;break;} // c0628294 lfs 3, -32108(2)
case 0x800104e8:{if(!store((r[6]+uint32_t(20)),r[0]))return {};c.pc=0x800104ec;break;} // 90060014 stw 0, 20(6)
case 0x800104ec:{uint32_t v;if(!word((r[2]+uint32_t(-32032)),v))return {};f[4]=bits(double(single(v)));c.paired[4]=v;c.pc=0x800104f0;break;} // c08282e0 lfs 4, -32032(2)
case 0x800104f0:{uint32_t v;if(!word((r[2]+uint32_t(-32072)),v))return {};f[5]=bits(double(single(v)));c.paired[5]=v;c.pc=0x800104f4;break;} // c0a282b8 lfs 5, -32072(2)
case 0x800104f4:{uint32_t v;if(!word((r[2]+uint32_t(-32028)),v))return {};f[6]=bits(double(single(v)));c.paired[6]=v;c.pc=0x800104f8;break;} // c0c282e4 lfs 6, -32028(2)
case 0x800104f8:{c.lr=0x800104fc;c.pc=0x8000fac8;break;} // 4bfff5d1 bl 0x8000fac8
case 0x800104fc:{r[4]=uint32_t(-32707)<<16;c.pc=0x80010500;break;} // 3c80803d lis 4, -32707
case 0x80010500:{uint32_t v;if(!word((r[2]+uint32_t(-32116)),v))return {};f[1]=bits(double(single(v)));c.paired[1]=v;c.pc=0x80010504;break;} // c022828c lfs 1, -32116(2)
case 0x80010504:{r[4]=r[4]+uint32_t(-27144);c.pc=0x80010508;break;} // 388495f8 addi 4, 4, -27144
case 0x80010508:{uint32_t v;if(!word((r[2]+uint32_t(-32104)),v))return {};f[2]=bits(double(single(v)));c.paired[2]=v;c.pc=0x8001050c;break;} // c0428298 lfs 2, -32104(2)
case 0x8001050c:{if(!store((r[4]+uint32_t(28)),r[3]))return {};c.pc=0x80010510;break;} // 9064001c stw 3, 28(4)
case 0x80010510:{r[3]=uint32_t(0);c.pc=0x80010514;break;} // 38600000 li 3, 0
case 0x80010514:{uint32_t v;if(!word((r[2]+uint32_t(-32108)),v))return {};f[3]=bits(double(single(v)));c.paired[3]=v;c.pc=0x80010518;break;} // c0628294 lfs 3, -32108(2)
case 0x80010518:{r[4]=r[2]+uint32_t(-32004);c.pc=0x8001051c;break;} // 388282fc addi 4, 2, -32004
case 0x8001051c:{uint32_t v;if(!word((r[2]+uint32_t(-32032)),v))return {};f[4]=bits(double(single(v)));c.paired[4]=v;c.pc=0x80010520;break;} // c08282e0 lfs 4, -32032(2)
case 0x80010520:{uint32_t v;if(!word((r[2]+uint32_t(-32008)),v))return {};f[5]=bits(double(single(v)));c.paired[5]=v;c.pc=0x80010524;break;} // c0a282f8 lfs 5, -32008(2)
case 0x80010524:{uint32_t v;if(!word((r[2]+uint32_t(-32072)),v))return {};f[6]=bits(double(single(v)));c.paired[6]=v;c.pc=0x80010528;break;} // c0c282b8 lfs 6, -32072(2)
case 0x80010528:{c.lr=0x8001052c;c.pc=0x8000fac8;break;} // 4bfff5a1 bl 0x8000fac8
case 0x8001052c:{r[4]=uint32_t(-32707)<<16;c.pc=0x80010530;break;} // 3c80803d lis 4, -32707
case 0x80010530:{if(!c.step){if(!word((r[30]+uint32_t(128)),c.transfer))return {};c.step=1;}uint32_t low;if(!word((r[30]+uint32_t(128))+4,low))return {};f[1]=(uint64_t(c.transfer)<<32)|low;c.step=0;c.pc=0x80010534;break;} // c83e0080 lfd 1, 128(30)
case 0x80010534:{r[31]=r[4]+uint32_t(-27144);c.pc=0x80010538;break;} // 3be495f8 addi 31, 4, -27144
case 0x80010538:{if(!store((r[31]+uint32_t(32)),r[3]))return {};c.pc=0x8001053c;break;} // 907f0020 stw 3, 32(31)
case 0x8001053c:{if(!external(0x802724e8,0x80010540))return {};break;} // 48261fad bl 0x802724e8
case 0x80010540:{f[1]=rounded(scalar(f[1]));c.pc=0x80010544;break;} // fc200818 frsp 1, 1
case 0x80010544:{uint32_t v;if(!word((r[2]+uint32_t(-32132)),v))return {};f[0]=bits(double(single(v)));c.paired[0]=v;c.pc=0x80010548;break;} // c002827c lfs 0, -32132(2)
case 0x80010548:{if(!word((r[30]+uint32_t(72)),r[0]))return {};c.pc=0x8001054c;break;} // 801e0048 lwz 0, 72(30)
case 0x8001054c:{r[3]=uint32_t(0);c.pc=0x80010550;break;} // 38600000 li 3, 0
case 0x80010550:{if(!word((r[30]+uint32_t(76)),r[5]))return {};c.pc=0x80010554;break;} // 80be004c lwz 5, 76(30)
case 0x80010554:{r[4]=r[2]+uint32_t(-32000);c.pc=0x80010558;break;} // 38828300 addi 4, 2, -32000
case 0x80010558:{f[0]=rounded(scalar(f[0])/scalar(f[1]));c.pc=0x8001055c;break;} // ec000824 fdivs 0, 0, 1
case 0x8001055c:{if(!store((r[1]+uint32_t(32)),r[0]))return {};c.pc=0x80010560;break;} // 90010020 stw 0, 32(1)
case 0x80010560:{uint32_t v;if(!word((r[2]+uint32_t(-32140)),v))return {};f[5]=bits(double(single(v)));c.paired[5]=v;c.pc=0x80010564;break;} // c0a28274 lfs 5, -32140(2)
case 0x80010564:{if(!word((r[30]+uint32_t(80)),r[0]))return {};c.pc=0x80010568;break;} // 801e0050 lwz 0, 80(30)
case 0x80010568:{if(!store((r[1]+uint32_t(36)),r[5]))return {};c.pc=0x8001056c;break;} // 90a10024 stw 5, 36(1)
case 0x8001056c:{if(!word((r[31]+uint32_t(32)),r[6]))return {};c.pc=0x80010570;break;} // 80df0020 lwz 6, 32(31)
case 0x80010570:{if(!store((r[1]+uint32_t(40)),r[0]))return {};c.pc=0x80010574;break;} // 90010028 stw 0, 40(1)
case 0x80010574:{uint32_t v;if(!word((r[2]+uint32_t(-32116)),v))return {};f[1]=bits(double(single(v)));c.paired[1]=v;c.pc=0x80010578;break;} // c022828c lfs 1, -32116(2)
case 0x80010578:{if(!store((r[1]+uint32_t(32)),singleBits(scalar(f[5]))))return {};c.pc=0x8001057c;break;} // d0a10020 stfs 5, 32(1)
case 0x8001057c:{uint32_t v;if(!word((r[2]+uint32_t(-32024)),v))return {};f[2]=bits(double(single(v)));c.paired[2]=v;c.pc=0x80010580;break;} // c04282e8 lfs 2, -32024(2)
case 0x80010580:{if(!word((r[1]+uint32_t(32)),r[0]))return {};c.pc=0x80010584;break;} // 80010020 lwz 0, 32(1)
case 0x80010584:{if(!store((r[1]+uint32_t(36)),singleBits(scalar(f[5]))))return {};c.pc=0x80010588;break;} // d0a10024 stfs 5, 36(1)
case 0x80010588:{uint32_t v;if(!word((r[2]+uint32_t(-32128)),v))return {};f[3]=bits(double(single(v)));c.paired[3]=v;c.pc=0x8001058c;break;} // c0628280 lfs 3, -32128(2)
case 0x8001058c:{if(!store((r[1]+uint32_t(40)),singleBits(scalar(f[0]))))return {};c.pc=0x80010590;break;} // d0010028 stfs 0, 40(1)
case 0x80010590:{if(!word((r[1]+uint32_t(36)),r[5]))return {};c.pc=0x80010594;break;} // 80a10024 lwz 5, 36(1)
case 0x80010594:{if(!store((r[6]+uint32_t(12)),r[0]))return {};c.pc=0x80010598;break;} // 9006000c stw 0, 12(6)
case 0x80010598:{if(!word((r[1]+uint32_t(40)),r[0]))return {};c.pc=0x8001059c;break;} // 80010028 lwz 0, 40(1)
case 0x8001059c:{if(!store((r[6]+uint32_t(16)),r[5]))return {};c.pc=0x800105a0;break;} // 90a60010 stw 5, 16(6)
case 0x800105a0:{uint32_t v;if(!word((r[2]+uint32_t(-32112)),v))return {};f[4]=bits(double(single(v)));c.paired[4]=v;c.pc=0x800105a4;break;} // c0828290 lfs 4, -32112(2)
case 0x800105a4:{if(!store((r[6]+uint32_t(20)),r[0]))return {};c.pc=0x800105a8;break;} // 90060014 stw 0, 20(6)
case 0x800105a8:{uint32_t v;if(!word((r[2]+uint32_t(-32008)),v))return {};f[6]=bits(double(single(v)));c.paired[6]=v;c.pc=0x800105ac;break;} // c0c282f8 lfs 6, -32008(2)
case 0x800105ac:{c.lr=0x800105b0;c.pc=0x8000fac8;break;} // 4bfff51d bl 0x8000fac8
case 0x800105b0:{r[4]=uint32_t(-32707)<<16;c.pc=0x800105b4;break;} // 3c80803d lis 4, -32707
case 0x800105b4:{if(!c.step){if(!word((r[30]+uint32_t(128)),c.transfer))return {};c.step=1;}uint32_t low;if(!word((r[30]+uint32_t(128))+4,low))return {};f[1]=(uint64_t(c.transfer)<<32)|low;c.step=0;c.pc=0x800105b8;break;} // c83e0080 lfd 1, 128(30)
case 0x800105b8:{r[31]=r[4]+uint32_t(-27144);c.pc=0x800105bc;break;} // 3be495f8 addi 31, 4, -27144
case 0x800105bc:{if(!store((r[31]+uint32_t(36)),r[3]))return {};c.pc=0x800105c0;break;} // 907f0024 stw 3, 36(31)
case 0x800105c0:{if(!external(0x802724e8,0x800105c4))return {};break;} // 48261f29 bl 0x802724e8
case 0x800105c4:{f[1]=rounded(scalar(f[1]));c.pc=0x800105c8;break;} // fc200818 frsp 1, 1
case 0x800105c8:{uint32_t v;if(!word((r[2]+uint32_t(-32012)),v))return {};f[0]=bits(double(single(v)));c.paired[0]=v;c.pc=0x800105cc;break;} // c00282f4 lfs 0, -32012(2)
case 0x800105cc:{if(!word((r[30]+uint32_t(72)),r[0]))return {};c.pc=0x800105d0;break;} // 801e0048 lwz 0, 72(30)
case 0x800105d0:{r[3]=uint32_t(0);c.pc=0x800105d4;break;} // 38600000 li 3, 0
case 0x800105d4:{if(!word((r[30]+uint32_t(76)),r[5]))return {};c.pc=0x800105d8;break;} // 80be004c lwz 5, 76(30)
case 0x800105d8:{r[4]=r[2]+uint32_t(-31992);c.pc=0x800105dc;break;} // 38828308 addi 4, 2, -31992
case 0x800105dc:{f[0]=rounded(scalar(f[0])/scalar(f[1]));c.pc=0x800105e0;break;} // ec000824 fdivs 0, 0, 1
case 0x800105e0:{if(!store((r[1]+uint32_t(20)),r[0]))return {};c.pc=0x800105e4;break;} // 90010014 stw 0, 20(1)
case 0x800105e4:{uint32_t v;if(!word((r[2]+uint32_t(-32140)),v))return {};f[5]=bits(double(single(v)));c.paired[5]=v;c.pc=0x800105e8;break;} // c0a28274 lfs 5, -32140(2)
case 0x800105e8:{if(!word((r[30]+uint32_t(80)),r[0]))return {};c.pc=0x800105ec;break;} // 801e0050 lwz 0, 80(30)
case 0x800105ec:{if(!store((r[1]+uint32_t(24)),r[5]))return {};c.pc=0x800105f0;break;} // 90a10018 stw 5, 24(1)
case 0x800105f0:{if(!word((r[31]+uint32_t(36)),r[6]))return {};c.pc=0x800105f4;break;} // 80df0024 lwz 6, 36(31)
case 0x800105f4:{if(!store((r[1]+uint32_t(28)),r[0]))return {};c.pc=0x800105f8;break;} // 9001001c stw 0, 28(1)
case 0x800105f8:{uint32_t v;if(!word((r[2]+uint32_t(-32116)),v))return {};f[1]=bits(double(single(v)));c.paired[1]=v;c.pc=0x800105fc;break;} // c022828c lfs 1, -32116(2)
case 0x800105fc:{if(!store((r[1]+uint32_t(20)),singleBits(scalar(f[5]))))return {};c.pc=0x80010600;break;} // d0a10014 stfs 5, 20(1)
case 0x80010600:{uint32_t v;if(!word((r[2]+uint32_t(-32024)),v))return {};f[2]=bits(double(single(v)));c.paired[2]=v;c.pc=0x80010604;break;} // c04282e8 lfs 2, -32024(2)
case 0x80010604:{if(!word((r[1]+uint32_t(20)),r[0]))return {};c.pc=0x80010608;break;} // 80010014 lwz 0, 20(1)
case 0x80010608:{if(!store((r[1]+uint32_t(24)),singleBits(scalar(f[5]))))return {};c.pc=0x8001060c;break;} // d0a10018 stfs 5, 24(1)
case 0x8001060c:{uint32_t v;if(!word((r[2]+uint32_t(-32128)),v))return {};f[3]=bits(double(single(v)));c.paired[3]=v;c.pc=0x80010610;break;} // c0628280 lfs 3, -32128(2)
case 0x80010610:{if(!store((r[1]+uint32_t(28)),singleBits(scalar(f[0]))))return {};c.pc=0x80010614;break;} // d001001c stfs 0, 28(1)
case 0x80010614:{if(!word((r[1]+uint32_t(24)),r[5]))return {};c.pc=0x80010618;break;} // 80a10018 lwz 5, 24(1)
case 0x80010618:{if(!store((r[6]+uint32_t(12)),r[0]))return {};c.pc=0x8001061c;break;} // 9006000c stw 0, 12(6)
case 0x8001061c:{if(!word((r[1]+uint32_t(28)),r[0]))return {};c.pc=0x80010620;break;} // 8001001c lwz 0, 28(1)
case 0x80010620:{if(!store((r[6]+uint32_t(16)),r[5]))return {};c.pc=0x80010624;break;} // 90a60010 stw 5, 16(6)
case 0x80010624:{uint32_t v;if(!word((r[2]+uint32_t(-32112)),v))return {};f[4]=bits(double(single(v)));c.paired[4]=v;c.pc=0x80010628;break;} // c0828290 lfs 4, -32112(2)
case 0x80010628:{if(!store((r[6]+uint32_t(20)),r[0]))return {};c.pc=0x8001062c;break;} // 90060014 stw 0, 20(6)
case 0x8001062c:{uint32_t v;if(!word((r[2]+uint32_t(-32008)),v))return {};f[6]=bits(double(single(v)));c.paired[6]=v;c.pc=0x80010630;break;} // c0c282f8 lfs 6, -32008(2)
case 0x80010630:{c.lr=0x80010634;c.pc=0x8000fac8;break;} // 4bfff499 bl 0x8000fac8
case 0x80010634:{r[4]=uint32_t(-32707)<<16;c.pc=0x80010638;break;} // 3c80803d lis 4, -32707
case 0x80010638:{if(!c.step){if(!word((r[30]+uint32_t(128)),c.transfer))return {};c.step=1;}uint32_t low;if(!word((r[30]+uint32_t(128))+4,low))return {};f[1]=(uint64_t(c.transfer)<<32)|low;c.step=0;c.pc=0x8001063c;break;} // c83e0080 lfd 1, 128(30)
case 0x8001063c:{r[31]=r[4]+uint32_t(-27144);c.pc=0x80010640;break;} // 3be495f8 addi 31, 4, -27144
case 0x80010640:{if(!store((r[31]+uint32_t(40)),r[3]))return {};c.pc=0x80010644;break;} // 907f0028 stw 3, 40(31)
case 0x80010644:{if(!external(0x802724e8,0x80010648))return {};break;} // 48261ea5 bl 0x802724e8
case 0x80010648:{f[1]=rounded(scalar(f[1]));c.pc=0x8001064c;break;} // fc200818 frsp 1, 1
case 0x8001064c:{uint32_t v;if(!word((r[2]+uint32_t(-32012)),v))return {};f[0]=bits(double(single(v)));c.paired[0]=v;c.pc=0x80010650;break;} // c00282f4 lfs 0, -32012(2)
case 0x80010650:{if(!word((r[30]+uint32_t(72)),r[6]))return {};c.pc=0x80010654;break;} // 80de0048 lwz 6, 72(30)
case 0x80010654:{r[3]=uint32_t(0);c.pc=0x80010658;break;} // 38600000 li 3, 0
case 0x80010658:{if(!word((r[30]+uint32_t(76)),r[5]))return {};c.pc=0x8001065c;break;} // 80be004c lwz 5, 76(30)
case 0x8001065c:{r[4]=uint32_t(608);c.pc=0x80010660;break;} // 38800260 li 4, 608
case 0x80010660:{f[0]=rounded(scalar(f[0])/scalar(f[1]));c.pc=0x80010664;break;} // ec000824 fdivs 0, 0, 1
case 0x80010664:{if(!word((r[30]+uint32_t(80)),r[0]))return {};c.pc=0x80010668;break;} // 801e0050 lwz 0, 80(30)
case 0x80010668:{if(!store((r[1]+uint32_t(8)),r[6]))return {};c.pc=0x8001066c;break;} // 90c10008 stw 6, 8(1)
case 0x8001066c:{uint32_t v;if(!word((r[2]+uint32_t(-32140)),v))return {};f[1]=bits(double(single(v)));c.paired[1]=v;c.pc=0x80010670;break;} // c0228274 lfs 1, -32140(2)
case 0x80010670:{if(!store((r[1]+uint32_t(12)),r[5]))return {};c.pc=0x80010674;break;} // 90a1000c stw 5, 12(1)
case 0x80010674:{if(!word((r[31]+uint32_t(40)),r[6]))return {};c.pc=0x80010678;break;} // 80df0028 lwz 6, 40(31)
case 0x80010678:{if(!store((r[1]+uint32_t(8)),singleBits(scalar(f[1]))))return {};c.pc=0x8001067c;break;} // d0210008 stfs 1, 8(1)
case 0x8001067c:{if(!store((r[1]+uint32_t(16)),r[0]))return {};c.pc=0x80010680;break;} // 90010010 stw 0, 16(1)
case 0x80010680:{if(!word((r[1]+uint32_t(8)),r[0]))return {};c.pc=0x80010684;break;} // 80010008 lwz 0, 8(1)
case 0x80010684:{if(!store((r[1]+uint32_t(12)),singleBits(scalar(f[1]))))return {};c.pc=0x80010688;break;} // d021000c stfs 1, 12(1)
case 0x80010688:{if(!store((r[1]+uint32_t(16)),singleBits(scalar(f[0]))))return {};c.pc=0x8001068c;break;} // d0010010 stfs 0, 16(1)
case 0x8001068c:{if(!word((r[1]+uint32_t(12)),r[5]))return {};c.pc=0x80010690;break;} // 80a1000c lwz 5, 12(1)
case 0x80010690:{if(!store((r[6]+uint32_t(12)),r[0]))return {};c.pc=0x80010694;break;} // 9006000c stw 0, 12(6)
case 0x80010694:{if(!word((r[1]+uint32_t(16)),r[0]))return {};c.pc=0x80010698;break;} // 80010010 lwz 0, 16(1)
case 0x80010698:{if(!store((r[6]+uint32_t(16)),r[5]))return {};c.pc=0x8001069c;break;} // 90a60010 stw 5, 16(6)
case 0x8001069c:{if(!store((r[6]+uint32_t(20)),r[0]))return {};c.pc=0x800106a0;break;} // 90060014 stw 0, 20(6)
case 0x800106a0:{if(!external(0x800300f0,0x800106a4))return {};break;} // 4801fa51 bl 0x800300f0
case 0x800106a4:{r[31]=r[3];c.pc=0x800106a8;break;} // 7c7f1b78 mr 31, 3
case 0x800106a8:{r[4]=uint32_t(0);c.pc=0x800106ac;break;} // 38800000 li 4, 0
case 0x800106ac:{r[5]=uint32_t(608);c.pc=0x800106b0;break;} // 38a00260 li 5, 608
case 0x800106b0:{if(!external(0x800050b4,0x800106b4))return {};break;} // 4bff4a05 bl 0x800050b4
case 0x800106b4:{uint32_t v;if(!word((r[2]+uint32_t(-32140)),v))return {};f[1]=bits(double(single(v)));c.paired[1]=v;c.pc=0x800106b8;break;} // c0228274 lfs 1, -32140(2)
case 0x800106b8:{r[3]=r[31]+uint32_t(348);c.pc=0x800106bc;break;} // 387f015c addi 3, 31, 348
case 0x800106bc:{uint32_t v;if(!word((r[2]+uint32_t(-32096)),v))return {};f[2]=bits(double(single(v)));c.paired[2]=v;c.pc=0x800106c0;break;} // c04282a0 lfs 2, -32096(2)
case 0x800106c0:{f[3]=f[1];c.pc=0x800106c4;break;} // fc600890 fmr 3, 1
case 0x800106c4:{uint32_t v;if(!word((r[2]+uint32_t(-32100)),v))return {};f[4]=bits(double(single(v)));c.paired[4]=v;c.pc=0x800106c8;break;} // c082829c lfs 4, -32100(2)
case 0x800106c8:{uint32_t v;if(!word((r[2]+uint32_t(-32128)),v))return {};f[5]=bits(double(single(v)));c.paired[5]=v;c.pc=0x800106cc;break;} // c0a28280 lfs 5, -32128(2)
case 0x800106cc:{uint32_t v;if(!word((r[2]+uint32_t(-32112)),v))return {};f[6]=bits(double(single(v)));c.paired[6]=v;c.pc=0x800106d0;break;} // c0c28290 lfs 6, -32112(2)
case 0x800106d0:{c.lr=0x800106d4;c.pc=0x8029e8b8;break;} // 4828e1e9 bl 0x8029e8b8
case 0x800106d4:{r[0]=uint32_t(1);c.pc=0x800106d8;break;} // 38000001 li 0, 1
case 0x800106d8:{uint32_t v;if(!word((r[2]+uint32_t(-32128)),v))return {};f[2]=bits(double(single(v)));c.paired[2]=v;c.pc=0x800106dc;break;} // c0428280 lfs 2, -32128(2)
case 0x800106dc:{if(!store((r[31]+uint32_t(412)),r[0]))return {};c.pc=0x800106e0;break;} // 901f019c stw 0, 412(31)
case 0x800106e0:{r[3]=r[31]+uint32_t(12);c.pc=0x800106e4;break;} // 387f000c addi 3, 31, 12
case 0x800106e4:{uint32_t v;if(!word((r[2]+uint32_t(-32112)),v))return {};f[1]=bits(double(single(v)));c.paired[1]=v;c.pc=0x800106e8;break;} // c0228290 lfs 1, -32112(2)
case 0x800106e8:{r[4]=r[31]+uint32_t(24);c.pc=0x800106ec;break;} // 389f0018 addi 4, 31, 24
case 0x800106ec:{if(!word((r[13]+uint32_t(-32624)),r[5]))return {};c.pc=0x800106f0;break;} // 80ad8090 lwz 5, -32624(13)
case 0x800106f0:{if(!word((r[13]+uint32_t(-32620)),r[0]))return {};c.pc=0x800106f4;break;} // 800d8094 lwz 0, -32620(13)
case 0x800106f4:{uint32_t v;if(!word((r[2]+uint32_t(-32140)),v))return {};f[0]=bits(double(single(v)));c.paired[0]=v;c.pc=0x800106f8;break;} // c0028274 lfs 0, -32140(2)
case 0x800106f8:{if(!store((r[31]+uint32_t(12)),r[5]))return {};c.pc=0x800106fc;break;} // 90bf000c stw 5, 12(31)
case 0x800106fc:{if(!store((r[31]+uint32_t(16)),r[0]))return {};c.pc=0x80010700;break;} // 901f0010 stw 0, 16(31)
case 0x80010700:{if(!word((r[13]+uint32_t(-32616)),r[0]))return {};c.pc=0x80010704;break;} // 800d8098 lwz 0, -32616(13)
case 0x80010704:{if(!store((r[31]+uint32_t(20)),r[0]))return {};c.pc=0x80010708;break;} // 901f0014 stw 0, 20(31)
case 0x80010708:{if(!word((r[13]+uint32_t(-32612)),r[5]))return {};c.pc=0x8001070c;break;} // 80ad809c lwz 5, -32612(13)
case 0x8001070c:{if(!word((r[13]+uint32_t(-32608)),r[0]))return {};c.pc=0x80010710;break;} // 800d80a0 lwz 0, -32608(13)
case 0x80010710:{if(!store((r[31]+uint32_t(24)),r[5]))return {};c.pc=0x80010714;break;} // 90bf0018 stw 5, 24(31)
case 0x80010714:{if(!store((r[31]+uint32_t(28)),r[0]))return {};c.pc=0x80010718;break;} // 901f001c stw 0, 28(31)
case 0x80010718:{if(!word((r[13]+uint32_t(-32604)),r[0]))return {};c.pc=0x8001071c;break;} // 800d80a4 lwz 0, -32604(13)
case 0x8001071c:{if(!store((r[31]+uint32_t(32)),r[0]))return {};c.pc=0x80010720;break;} // 901f0020 stw 0, 32(31)
case 0x80010720:{if(!word((r[13]+uint32_t(-32600)),r[5]))return {};c.pc=0x80010724;break;} // 80ad80a8 lwz 5, -32600(13)
case 0x80010724:{if(!word((r[13]+uint32_t(-32596)),r[0]))return {};c.pc=0x80010728;break;} // 800d80ac lwz 0, -32596(13)
case 0x80010728:{if(!store((r[31]+uint32_t(36)),r[5]))return {};c.pc=0x8001072c;break;} // 90bf0024 stw 5, 36(31)
case 0x8001072c:{if(!store((r[31]+uint32_t(40)),r[0]))return {};c.pc=0x80010730;break;} // 901f0028 stw 0, 40(31)
case 0x80010730:{if(!word((r[13]+uint32_t(-32592)),r[0]))return {};c.pc=0x80010734;break;} // 800d80b0 lwz 0, -32592(13)
case 0x80010734:{if(!store((r[31]+uint32_t(44)),r[0]))return {};c.pc=0x80010738;break;} // 901f002c stw 0, 44(31)
case 0x80010738:{if(!store((r[31]+uint32_t(48)),singleBits(scalar(f[2]))))return {};c.pc=0x8001073c;break;} // d05f0030 stfs 2, 48(31)
case 0x8001073c:{if(!store((r[31]+uint32_t(52)),singleBits(scalar(f[1]))))return {};c.pc=0x80010740;break;} // d03f0034 stfs 1, 52(31)
case 0x80010740:{if(!store((r[31]+uint32_t(276)),singleBits(scalar(f[0]))))return {};c.pc=0x80010744;break;} // d01f0114 stfs 0, 276(31)
case 0x80010744:{if(!external(0x8029eeb4,0x80010748))return {};break;} // 4828e771 bl 0x8029eeb4
case 0x80010748:{if(!store((r[31]+uint32_t(280)),singleBits(scalar(f[1]))))return {};c.pc=0x8001074c;break;} // d03f0118 stfs 1, 280(31)
case 0x8001074c:{r[5]=uint32_t(0);c.pc=0x80010750;break;} // 38a00000 li 5, 0
case 0x80010750:{uint32_t v;if(!word((r[2]+uint32_t(-32140)),v))return {};f[2]=bits(double(single(v)));c.paired[2]=v;c.pc=0x80010754;break;} // c0428274 lfs 2, -32140(2)
case 0x80010754:{r[0]=uint32_t(8);c.pc=0x80010758;break;} // 38000008 li 0, 8
case 0x80010758:{if(!store((r[31]+uint32_t(492)),r[5]))return {};c.pc=0x8001075c;break;} // 90bf01ec stw 5, 492(31)
case 0x8001075c:{r[3]=r[31]+uint32_t(596);c.pc=0x80010760;break;} // 387f0254 addi 3, 31, 596
case 0x80010760:{uint32_t v;if(!word((r[2]+uint32_t(-32104)),v))return {};f[1]=bits(double(single(v)));c.paired[1]=v;c.pc=0x80010764;break;} // c0228298 lfs 1, -32104(2)
case 0x80010764:{r[4]=r[2]+uint32_t(-31984);c.pc=0x80010768;break;} // 38828310 addi 4, 2, -31984
case 0x80010768:{if(!s.storeHalf||!s.storeHalf(s.user,(r[31]+uint32_t(2)),uint16_t(r[5])))return {};c.pc=0x8001076c;break;} // b0bf0002 sth 5, 2(31)
case 0x8001076c:{uint32_t v;if(!word((r[2]+uint32_t(-32116)),v))return {};f[0]=bits(double(single(v)));c.paired[0]=v;c.pc=0x80010770;break;} // c002828c lfs 0, -32116(2)
case 0x80010770:{if(!store((r[31]+uint32_t(332)),singleBits(scalar(f[2]))))return {};c.pc=0x80010774;break;} // d05f014c stfs 2, 332(31)
case 0x80010774:{if(!store((r[31]+uint32_t(344)),singleBits(scalar(f[2]))))return {};c.pc=0x80010778;break;} // d05f0158 stfs 2, 344(31)
case 0x80010778:{if(!store((r[31]+uint32_t(340)),singleBits(scalar(f[2]))))return {};c.pc=0x8001077c;break;} // d05f0154 stfs 2, 340(31)
case 0x8001077c:{if(!store((r[31]+uint32_t(336)),singleBits(scalar(f[2]))))return {};c.pc=0x80010780;break;} // d05f0150 stfs 2, 336(31)
case 0x80010780:{if(!store((r[31]+uint32_t(60)),singleBits(scalar(f[1]))))return {};c.pc=0x80010784;break;} // d03f003c stfs 1, 60(31)
case 0x80010784:{if(!store((r[31]+uint32_t(56)),singleBits(scalar(f[0]))))return {};c.pc=0x80010788;break;} // d01f0038 stfs 0, 56(31)
case 0x80010788:{if(!store((r[31]+uint32_t(488)),r[0]))return {};c.pc=0x8001078c;break;} // 901f01e8 stw 0, 488(31)
case 0x8001078c:{if(!external(0x8026d5fc,0x80010790))return {};break;} // 4825ce71 bl 0x8026d5fc
case 0x80010790:{r[0]=uint32_t(0);c.pc=0x80010794;break;} // 38000000 li 0, 0
case 0x80010794:{if(!s.storeHalf||!s.storeHalf(s.user,(r[31]+uint32_t(244)),uint16_t(r[0])))return {};c.pc=0x80010798;break;} // b01f00f4 sth 0, 244(31)
case 0x80010798:{if(!s.storeHalf||!s.storeHalf(s.user,(r[31]+uint32_t(246)),uint16_t(r[0])))return {};c.pc=0x8001079c;break;} // b01f00f6 sth 0, 246(31)
case 0x8001079c:{c.lr=0x800107a0;c.pc=0x8002eb30;break;} // 4801e395 bl 0x8002eb30
case 0x800107a0:{if(!half((r[3]+uint32_t(4)),r[0]))return {};c.pc=0x800107a4;break;} // a0030004 lhz 0, 4(3)
case 0x800107a4:{if(!s.storeHalf||!s.storeHalf(s.user,(r[31]+uint32_t(248)),uint16_t(r[0])))return {};c.pc=0x800107a8;break;} // b01f00f8 sth 0, 248(31)
case 0x800107a8:{c.lr=0x800107ac;c.pc=0x8002eb30;break;} // 4801e389 bl 0x8002eb30
case 0x800107ac:{if(!half((r[3]+uint32_t(6)),r[0]))return {};c.pc=0x800107b0;break;} // a0030006 lhz 0, 6(3)
case 0x800107b0:{uint32_t v;if(!word((r[2]+uint32_t(-32140)),v))return {};f[0]=bits(double(single(v)));c.paired[0]=v;c.pc=0x800107b4;break;} // c0028274 lfs 0, -32140(2)
case 0x800107b4:{if(!s.storeHalf||!s.storeHalf(s.user,(r[31]+uint32_t(250)),uint16_t(r[0])))return {};c.pc=0x800107b8;break;} // b01f00fa sth 0, 250(31)
case 0x800107b8:{if(!store((r[31]+uint32_t(252)),singleBits(scalar(f[0]))))return {};c.pc=0x800107bc;break;} // d01f00fc stfs 0, 252(31)
case 0x800107bc:{if(!store((r[31]+uint32_t(256)),singleBits(scalar(f[0]))))return {};c.pc=0x800107c0;break;} // d01f0100 stfs 0, 256(31)
case 0x800107c0:{c.lr=0x800107c4;c.pc=0x8002eb30;break;} // 4801e371 bl 0x8002eb30
case 0x800107c4:{if(!half((r[3]+uint32_t(4)),r[3]))return {};c.pc=0x800107c8;break;} // a0630004 lhz 3, 4(3)
case 0x800107c8:{r[0]=uint32_t(17200)<<16;c.pc=0x800107cc;break;} // 3c004330 lis 0, 17200
case 0x800107cc:{if(!store((r[1]+uint32_t(120)),r[0]))return {};c.pc=0x800107d0;break;} // 90010078 stw 0, 120(1)
case 0x800107d0:{if(!c.step){if(!word((r[30]+uint32_t(104)),c.transfer))return {};c.step=1;}uint32_t low;if(!word((r[30]+uint32_t(104))+4,low))return {};f[1]=(uint64_t(c.transfer)<<32)|low;c.step=0;c.pc=0x800107d4;break;} // c83e0068 lfd 1, 104(30)
case 0x800107d4:{if(!store((r[1]+uint32_t(124)),r[3]))return {};c.pc=0x800107d8;break;} // 9061007c stw 3, 124(1)
case 0x800107d8:{if(!c.step){if(!word((r[1]+uint32_t(120)),c.transfer))return {};c.step=1;}uint32_t low;if(!word((r[1]+uint32_t(120))+4,low))return {};f[0]=(uint64_t(c.transfer)<<32)|low;c.step=0;c.pc=0x800107dc;break;} // c8010078 lfd 0, 120(1)
case 0x800107dc:{f[0]=rounded(scalar(f[0])-scalar(f[1]));c.pc=0x800107e0;break;} // ec000828 fsubs 0, 0, 1
case 0x800107e0:{if(!store((r[31]+uint32_t(260)),singleBits(scalar(f[0]))))return {};c.pc=0x800107e4;break;} // d01f0104 stfs 0, 260(31)
case 0x800107e4:{c.lr=0x800107e8;c.pc=0x8002eb30;break;} // 4801e34d bl 0x8002eb30
case 0x800107e8:{if(!half((r[3]+uint32_t(6)),r[4]))return {};c.pc=0x800107ec;break;} // a0830006 lhz 4, 6(3)
case 0x800107ec:{r[0]=uint32_t(17200)<<16;c.pc=0x800107f0;break;} // 3c004330 lis 0, 17200
case 0x800107f0:{uint32_t v;if(!word((r[2]+uint32_t(-32128)),v))return {};f[3]=bits(double(single(v)));c.paired[3]=v;c.pc=0x800107f4;break;} // c0628280 lfs 3, -32128(2)
case 0x800107f4:{r[3]=uint32_t(-32707)<<16;c.pc=0x800107f8;break;} // 3c60803d lis 3, -32707
case 0x800107f8:{if(!store((r[1]+uint32_t(132)),r[4]))return {};c.pc=0x800107fc;break;} // 90810084 stw 4, 132(1)
case 0x800107fc:{r[5]=r[3]+uint32_t(-27144);c.pc=0x80010800;break;} // 38a395f8 addi 5, 3, -27144
case 0x80010800:{if(!c.step){if(!word((r[30]+uint32_t(104)),c.transfer))return {};c.step=1;}uint32_t low;if(!word((r[30]+uint32_t(104))+4,low))return {};f[2]=(uint64_t(c.transfer)<<32)|low;c.step=0;c.pc=0x80010804;break;} // c85e0068 lfd 2, 104(30)
case 0x80010804:{f[6]=f[3];c.pc=0x80010808;break;} // fcc01890 fmr 6, 3
case 0x80010808:{if(!store((r[1]+uint32_t(128)),r[0]))return {};c.pc=0x8001080c;break;} // 90010080 stw 0, 128(1)
case 0x8001080c:{r[3]=uint32_t(0);c.pc=0x80010810;break;} // 38600000 li 3, 0
case 0x80010810:{uint32_t v;if(!word((r[2]+uint32_t(-32140)),v))return {};f[5]=bits(double(single(v)));c.paired[5]=v;c.pc=0x80010814;break;} // c0a28274 lfs 5, -32140(2)
case 0x80010814:{r[4]=r[2]+uint32_t(-31980);c.pc=0x80010818;break;} // 38828314 addi 4, 2, -31980
case 0x80010818:{if(!c.step){if(!word((r[1]+uint32_t(128)),c.transfer))return {};c.step=1;}uint32_t low;if(!word((r[1]+uint32_t(128))+4,low))return {};f[0]=(uint64_t(c.transfer)<<32)|low;c.step=0;c.pc=0x8001081c;break;} // c8010080 lfd 0, 128(1)
case 0x8001081c:{uint32_t v;if(!word((r[2]+uint32_t(-32116)),v))return {};f[1]=bits(double(single(v)));c.paired[1]=v;c.pc=0x80010820;break;} // c022828c lfs 1, -32116(2)
case 0x80010820:{f[0]=rounded(scalar(f[0])-scalar(f[2]));c.pc=0x80010824;break;} // ec001028 fsubs 0, 0, 2
case 0x80010824:{uint32_t v;if(!word((r[2]+uint32_t(-32104)),v))return {};f[2]=bits(double(single(v)));c.paired[2]=v;c.pc=0x80010828;break;} // c0428298 lfs 2, -32104(2)
case 0x80010828:{uint32_t v;if(!word((r[2]+uint32_t(-32112)),v))return {};f[4]=bits(double(single(v)));c.paired[4]=v;c.pc=0x8001082c;break;} // c0828290 lfs 4, -32112(2)
case 0x8001082c:{if(!store((r[31]+uint32_t(264)),singleBits(scalar(f[0]))))return {};c.pc=0x80010830;break;} // d01f0108 stfs 0, 264(31)
case 0x80010830:{if(!store((r[31]+uint32_t(268)),singleBits(scalar(f[5]))))return {};c.pc=0x80010834;break;} // d0bf010c stfs 5, 268(31)
case 0x80010834:{if(!store((r[31]+uint32_t(272)),singleBits(scalar(f[3]))))return {};c.pc=0x80010838;break;} // d07f0110 stfs 3, 272(31)
case 0x80010838:{if(!store((r[5]+uint32_t(44)),r[31]))return {};c.pc=0x8001083c;break;} // 93e5002c stw 31, 44(5)
case 0x8001083c:{c.lr=0x80010840;c.pc=0x8000fac8;break;} // 4bfff28d bl 0x8000fac8
case 0x80010840:{r[4]=uint32_t(-32707)<<16;c.pc=0x80010844;break;} // 3c80803d lis 4, -32707
case 0x80010844:{r[4]=r[4]+uint32_t(-27144);c.pc=0x80010848;break;} // 388495f8 addi 4, 4, -27144
case 0x80010848:{if(!store((r[4]+uint32_t(48)),r[3]))return {};c.pc=0x8001084c;break;} // 90640030 stw 3, 48(4)
case 0x8001084c:{if(!word((r[1]+uint32_t(140)),r[31]))return {};c.pc=0x80010850;break;} // 83e1008c lwz 31, 140(1)
case 0x80010850:{if(!word((r[1]+uint32_t(136)),r[30]))return {};c.pc=0x80010854;break;} // 83c10088 lwz 30, 136(1)
case 0x80010854:{if(!word((r[1]+uint32_t(148)),r[0]))return {};c.pc=0x80010858;break;} // 80010094 lwz 0, 148(1)
case 0x80010858:{c.lr=r[0];c.pc=0x8001085c;break;} // 7c0803a6 mtlr 0
case 0x8001085c:{r[1]=r[1]+uint32_t(144);c.pc=0x80010860;break;} // 38210090 addi 1, 1, 144
case 0x80010860:{c.pc=c.lr;break;} // 4e800020 blr 
case 0x8000fac8:{const auto target=(r[1]+uint32_t(-144));if(!store(target,r[1]))return {};r[1]=target;c.pc=0x8000facc;break;} // 9421ff70 stwu 1, -144(1)
case 0x8000facc:{r[0]=c.lr;c.pc=0x8000fad0;break;} // 7c0802a6 mflr 0
case 0x8000fad0:{if(!store((r[1]+uint32_t(148)),r[0]))return {};c.pc=0x8000fad4;break;} // 90010094 stw 0, 148(1)
case 0x8000fad4:{while(c.step<2){if(!store((r[1]+uint32_t(128))+4*c.step,uint32_t(f[31]>>(c.step?0:32))))return {};++c.step;}c.step=0;c.pc=0x8000fad8;break;} // dbe10080 stfd 31, 128(1)
case 0x8000fad8:{while(c.step<2){if(!store((r[1]+uint32_t(136))+4*c.step,c.step?c.paired[31]:singleBits(scalar(f[31]))))return {};++c.step;}c.step=0;c.pc=0x8000fadc;break;} // f3e10088 psq_st f31, 136(r1), 0, 0
case 0x8000fadc:{while(c.step<2){if(!store((r[1]+uint32_t(112))+4*c.step,uint32_t(f[30]>>(c.step?0:32))))return {};++c.step;}c.step=0;c.pc=0x8000fae0;break;} // dbc10070 stfd 30, 112(1)
case 0x8000fae0:{while(c.step<2){if(!store((r[1]+uint32_t(120))+4*c.step,c.step?c.paired[30]:singleBits(scalar(f[30]))))return {};++c.step;}c.step=0;c.pc=0x8000fae4;break;} // f3c10078 psq_st f30, 120(r1), 0, 0
case 0x8000fae4:{while(c.step<2){if(!store((r[1]+uint32_t(96))+4*c.step,uint32_t(f[29]>>(c.step?0:32))))return {};++c.step;}c.step=0;c.pc=0x8000fae8;break;} // dba10060 stfd 29, 96(1)
case 0x8000fae8:{while(c.step<2){if(!store((r[1]+uint32_t(104))+4*c.step,c.step?c.paired[29]:singleBits(scalar(f[29]))))return {};++c.step;}c.step=0;c.pc=0x8000faec;break;} // f3a10068 psq_st f29, 104(r1), 0, 0
case 0x8000faec:{while(c.step<2){if(!store((r[1]+uint32_t(80))+4*c.step,uint32_t(f[28]>>(c.step?0:32))))return {};++c.step;}c.step=0;c.pc=0x8000faf0;break;} // db810050 stfd 28, 80(1)
case 0x8000faf0:{while(c.step<2){if(!store((r[1]+uint32_t(88))+4*c.step,c.step?c.paired[28]:singleBits(scalar(f[28]))))return {};++c.step;}c.step=0;c.pc=0x8000faf4;break;} // f3810058 psq_st f28, 88(r1), 0, 0
case 0x8000faf4:{while(c.step<2){if(!store((r[1]+uint32_t(64))+4*c.step,uint32_t(f[27]>>(c.step?0:32))))return {};++c.step;}c.step=0;c.pc=0x8000faf8;break;} // db610040 stfd 27, 64(1)
case 0x8000faf8:{while(c.step<2){if(!store((r[1]+uint32_t(72))+4*c.step,c.step?c.paired[27]:singleBits(scalar(f[27]))))return {};++c.step;}c.step=0;c.pc=0x8000fafc;break;} // f3610048 psq_st f27, 72(r1), 0, 0
case 0x8000fafc:{while(c.step<2){if(!store((r[1]+uint32_t(48))+4*c.step,uint32_t(f[26]>>(c.step?0:32))))return {};++c.step;}c.step=0;c.pc=0x8000fb00;break;} // db410030 stfd 26, 48(1)
case 0x8000fb00:{while(c.step<2){if(!store((r[1]+uint32_t(56))+4*c.step,c.step?c.paired[26]:singleBits(scalar(f[26]))))return {};++c.step;}c.step=0;c.pc=0x8000fb04;break;} // f3410038 psq_st f26, 56(r1), 0, 0
case 0x8000fb04:{if(!store((r[1]+uint32_t(44)),r[31]))return {};c.pc=0x8000fb08;break;} // 93e1002c stw 31, 44(1)
case 0x8000fb08:{if(!store((r[1]+uint32_t(40)),r[30]))return {};c.pc=0x8000fb0c;break;} // 93c10028 stw 30, 40(1)
case 0x8000fb0c:{if(!store((r[1]+uint32_t(36)),r[29]))return {};c.pc=0x8000fb10;break;} // 93a10024 stw 29, 36(1)
case 0x8000fb10:{if(!store((r[1]+uint32_t(32)),r[28]))return {};c.pc=0x8000fb14;break;} // 93810020 stw 28, 32(1)
case 0x8000fb14:{f[26]=f[1];c.pc=0x8000fb18;break;} // ff400890 fmr 26, 1
case 0x8000fb18:{r[5]=uint32_t(-32724)<<16;c.pc=0x8000fb1c;break;} // 3ca0802c lis 5, -32724
case 0x8000fb1c:{f[27]=f[2];c.pc=0x8000fb20;break;} // ff601090 fmr 27, 2
case 0x8000fb20:{r[28]=r[3];c.pc=0x8000fb24;break;} // 7c7c1b78 mr 28, 3
case 0x8000fb24:{r[29]=r[4];c.pc=0x8000fb28;break;} // 7c9d2378 mr 29, 4
case 0x8000fb28:{f[28]=f[3];c.pc=0x8000fb2c;break;} // ff801890 fmr 28, 3
case 0x8000fb2c:{f[29]=f[4];c.pc=0x8000fb30;break;} // ffa02090 fmr 29, 4
case 0x8000fb30:{r[31]=r[5]+uint32_t(-2600);c.pc=0x8000fb34;break;} // 3be5f5d8 addi 31, 5, -2600
case 0x8000fb34:{f[30]=f[5];c.pc=0x8000fb38;break;} // ffc02890 fmr 30, 5
case 0x8000fb38:{r[3]=uint32_t(0);c.pc=0x8000fb3c;break;} // 38600000 li 3, 0
case 0x8000fb3c:{f[31]=f[6];c.pc=0x8000fb40;break;} // ffe03090 fmr 31, 6
case 0x8000fb40:{r[4]=uint32_t(608);c.pc=0x8000fb44;break;} // 38800260 li 4, 608
case 0x8000fb44:{if(!external(0x800300f0,0x8000fb48))return {};break;} // 480205ad bl 0x800300f0
case 0x8000fb48:{r[4]=uint32_t(0);c.pc=0x8000fb4c;break;} // 38800000 li 4, 0
case 0x8000fb4c:{r[30]=r[3];c.pc=0x8000fb50;break;} // 7c7e1b78 mr 30, 3
case 0x8000fb50:{r[5]=uint32_t(608);c.pc=0x8000fb54;break;} // 38a00260 li 5, 608
case 0x8000fb54:{if(!external(0x800050b4,0x8000fb58))return {};break;} // 4bff5561 bl 0x800050b4
case 0x8000fb58:{f[1]=f[26];c.pc=0x8000fb5c;break;} // fc20d090 fmr 1, 26
case 0x8000fb5c:{r[3]=r[30]+uint32_t(348);c.pc=0x8000fb60;break;} // 387e015c addi 3, 30, 348
case 0x8000fb60:{f[2]=f[27];c.pc=0x8000fb64;break;} // fc40d890 fmr 2, 27
case 0x8000fb64:{f[3]=f[28];c.pc=0x8000fb68;break;} // fc60e090 fmr 3, 28
case 0x8000fb68:{f[4]=f[29];c.pc=0x8000fb6c;break;} // fc80e890 fmr 4, 29
case 0x8000fb6c:{c.lr=0x8000fb70;c.pc=0x8029e7e8;break;} // 4828ec7d bl 0x8029e7e8
case 0x8000fb70:{r[0]=uint32_t(0);c.pc=0x8000fb74;break;} // 38000000 li 0, 0
case 0x8000fb74:{uint32_t v;if(!word((r[2]+uint32_t(-32140)),v))return {};f[0]=bits(double(single(v)));c.paired[0]=v;c.pc=0x8000fb78;break;} // c0028274 lfs 0, -32140(2)
case 0x8000fb78:{if(!store((r[30]+uint32_t(412)),r[0]))return {};c.pc=0x8000fb7c;break;} // 901e019c stw 0, 412(30)
case 0x8000fb7c:{r[3]=r[30]+uint32_t(12);c.pc=0x8000fb80;break;} // 387e000c addi 3, 30, 12
case 0x8000fb80:{r[4]=r[30]+uint32_t(24);c.pc=0x8000fb84;break;} // 389e0018 addi 4, 30, 24
case 0x8000fb84:{if(!word((r[13]+uint32_t(-32624)),r[5]))return {};c.pc=0x8000fb88;break;} // 80ad8090 lwz 5, -32624(13)
case 0x8000fb88:{if(!word((r[13]+uint32_t(-32620)),r[0]))return {};c.pc=0x8000fb8c;break;} // 800d8094 lwz 0, -32620(13)
case 0x8000fb8c:{if(!store((r[30]+uint32_t(12)),r[5]))return {};c.pc=0x8000fb90;break;} // 90be000c stw 5, 12(30)
case 0x8000fb90:{if(!store((r[30]+uint32_t(16)),r[0]))return {};c.pc=0x8000fb94;break;} // 901e0010 stw 0, 16(30)
case 0x8000fb94:{if(!word((r[13]+uint32_t(-32616)),r[0]))return {};c.pc=0x8000fb98;break;} // 800d8098 lwz 0, -32616(13)
case 0x8000fb98:{if(!store((r[30]+uint32_t(20)),r[0]))return {};c.pc=0x8000fb9c;break;} // 901e0014 stw 0, 20(30)
case 0x8000fb9c:{if(!word((r[13]+uint32_t(-32612)),r[5]))return {};c.pc=0x8000fba0;break;} // 80ad809c lwz 5, -32612(13)
case 0x8000fba0:{if(!word((r[13]+uint32_t(-32608)),r[0]))return {};c.pc=0x8000fba4;break;} // 800d80a0 lwz 0, -32608(13)
case 0x8000fba4:{if(!store((r[30]+uint32_t(24)),r[5]))return {};c.pc=0x8000fba8;break;} // 90be0018 stw 5, 24(30)
case 0x8000fba8:{if(!store((r[30]+uint32_t(28)),r[0]))return {};c.pc=0x8000fbac;break;} // 901e001c stw 0, 28(30)
case 0x8000fbac:{if(!word((r[13]+uint32_t(-32604)),r[0]))return {};c.pc=0x8000fbb0;break;} // 800d80a4 lwz 0, -32604(13)
case 0x8000fbb0:{if(!store((r[30]+uint32_t(32)),r[0]))return {};c.pc=0x8000fbb4;break;} // 901e0020 stw 0, 32(30)
case 0x8000fbb4:{if(!word((r[13]+uint32_t(-32600)),r[5]))return {};c.pc=0x8000fbb8;break;} // 80ad80a8 lwz 5, -32600(13)
case 0x8000fbb8:{if(!word((r[13]+uint32_t(-32596)),r[0]))return {};c.pc=0x8000fbbc;break;} // 800d80ac lwz 0, -32596(13)
case 0x8000fbbc:{if(!store((r[30]+uint32_t(36)),r[5]))return {};c.pc=0x8000fbc0;break;} // 90be0024 stw 5, 36(30)
case 0x8000fbc0:{if(!store((r[30]+uint32_t(40)),r[0]))return {};c.pc=0x8000fbc4;break;} // 901e0028 stw 0, 40(30)
case 0x8000fbc4:{if(!word((r[13]+uint32_t(-32592)),r[0]))return {};c.pc=0x8000fbc8;break;} // 800d80b0 lwz 0, -32592(13)
case 0x8000fbc8:{if(!store((r[30]+uint32_t(44)),r[0]))return {};c.pc=0x8000fbcc;break;} // 901e002c stw 0, 44(30)
case 0x8000fbcc:{if(!store((r[30]+uint32_t(48)),singleBits(scalar(f[28]))))return {};c.pc=0x8000fbd0;break;} // d39e0030 stfs 28, 48(30)
case 0x8000fbd0:{if(!store((r[30]+uint32_t(52)),singleBits(scalar(f[29]))))return {};c.pc=0x8000fbd4;break;} // d3be0034 stfs 29, 52(30)
case 0x8000fbd4:{if(!store((r[30]+uint32_t(276)),singleBits(scalar(f[0]))))return {};c.pc=0x8000fbd8;break;} // d01e0114 stfs 0, 276(30)
case 0x8000fbd8:{if(!external(0x8029eeb4,0x8000fbdc))return {};break;} // 4828f2dd bl 0x8029eeb4
case 0x8000fbdc:{if(!store((r[30]+uint32_t(280)),singleBits(scalar(f[1]))))return {};c.pc=0x8000fbe0;break;} // d03e0118 stfs 1, 280(30)
case 0x8000fbe0:{r[5]=uint32_t(0);c.pc=0x8000fbe4;break;} // 38a00000 li 5, 0
case 0x8000fbe4:{uint32_t v;if(!word((r[2]+uint32_t(-32140)),v))return {};f[0]=bits(double(single(v)));c.paired[0]=v;c.pc=0x8000fbe8;break;} // c0028274 lfs 0, -32140(2)
case 0x8000fbe8:{r[0]=uint32_t(8);c.pc=0x8000fbec;break;} // 38000008 li 0, 8
case 0x8000fbec:{if(!store((r[30]+uint32_t(492)),r[28]))return {};c.pc=0x8000fbf0;break;} // 939e01ec stw 28, 492(30)
case 0x8000fbf0:{r[4]=r[29];c.pc=0x8000fbf4;break;} // 7fa4eb78 mr 4, 29
case 0x8000fbf4:{r[3]=r[30]+uint32_t(596);c.pc=0x8000fbf8;break;} // 387e0254 addi 3, 30, 596
case 0x8000fbf8:{if(!s.storeHalf||!s.storeHalf(s.user,(r[30]+uint32_t(2)),uint16_t(r[5])))return {};c.pc=0x8000fbfc;break;} // b0be0002 sth 5, 2(30)
case 0x8000fbfc:{if(!s.storeHalf||!s.storeHalf(s.user,(r[30]+uint32_t(4)),uint16_t(r[5])))return {};c.pc=0x8000fc00;break;} // b0be0004 sth 5, 4(30)
case 0x8000fc00:{if(!store((r[30]+uint32_t(8)),singleBits(scalar(f[0]))))return {};c.pc=0x8000fc04;break;} // d01e0008 stfs 0, 8(30)
case 0x8000fc04:{if(!store((r[30]+uint32_t(56)),singleBits(scalar(f[26]))))return {};c.pc=0x8000fc08;break;} // d35e0038 stfs 26, 56(30)
case 0x8000fc08:{if(!store((r[30]+uint32_t(60)),singleBits(scalar(f[27]))))return {};c.pc=0x8000fc0c;break;} // d37e003c stfs 27, 60(30)
case 0x8000fc0c:{if(!store((r[30]+uint32_t(332)),singleBits(scalar(f[0]))))return {};c.pc=0x8000fc10;break;} // d01e014c stfs 0, 332(30)
case 0x8000fc10:{if(!store((r[30]+uint32_t(344)),singleBits(scalar(f[0]))))return {};c.pc=0x8000fc14;break;} // d01e0158 stfs 0, 344(30)
case 0x8000fc14:{if(!store((r[30]+uint32_t(340)),singleBits(scalar(f[0]))))return {};c.pc=0x8000fc18;break;} // d01e0154 stfs 0, 340(30)
case 0x8000fc18:{if(!store((r[30]+uint32_t(336)),singleBits(scalar(f[0]))))return {};c.pc=0x8000fc1c;break;} // d01e0150 stfs 0, 336(30)
case 0x8000fc1c:{if(!store((r[30]+uint32_t(488)),r[0]))return {};c.pc=0x8000fc20;break;} // 901e01e8 stw 0, 488(30)
case 0x8000fc20:{if(!external(0x8026d5fc,0x8000fc24))return {};break;} // 4825d9dd bl 0x8026d5fc
case 0x8000fc24:{r[0]=uint32_t(0);c.pc=0x8000fc28;break;} // 38000000 li 0, 0
case 0x8000fc28:{if(!s.storeHalf||!s.storeHalf(s.user,(r[30]+uint32_t(244)),uint16_t(r[0])))return {};c.pc=0x8000fc2c;break;} // b01e00f4 sth 0, 244(30)
case 0x8000fc2c:{if(!s.storeHalf||!s.storeHalf(s.user,(r[30]+uint32_t(246)),uint16_t(r[0])))return {};c.pc=0x8000fc30;break;} // b01e00f6 sth 0, 246(30)
case 0x8000fc30:{c.lr=0x8000fc34;c.pc=0x8002eb30;break;} // 4801ef01 bl 0x8002eb30
case 0x8000fc34:{if(!half((r[3]+uint32_t(4)),r[0]))return {};c.pc=0x8000fc38;break;} // a0030004 lhz 0, 4(3)
case 0x8000fc38:{if(!s.storeHalf||!s.storeHalf(s.user,(r[30]+uint32_t(248)),uint16_t(r[0])))return {};c.pc=0x8000fc3c;break;} // b01e00f8 sth 0, 248(30)
case 0x8000fc3c:{c.lr=0x8000fc40;c.pc=0x8002eb30;break;} // 4801eef5 bl 0x8002eb30
case 0x8000fc40:{if(!half((r[3]+uint32_t(6)),r[0]))return {};c.pc=0x8000fc44;break;} // a0030006 lhz 0, 6(3)
case 0x8000fc44:{uint32_t v;if(!word((r[2]+uint32_t(-32140)),v))return {};f[0]=bits(double(single(v)));c.paired[0]=v;c.pc=0x8000fc48;break;} // c0028274 lfs 0, -32140(2)
case 0x8000fc48:{if(!s.storeHalf||!s.storeHalf(s.user,(r[30]+uint32_t(250)),uint16_t(r[0])))return {};c.pc=0x8000fc4c;break;} // b01e00fa sth 0, 250(30)
case 0x8000fc4c:{if(!store((r[30]+uint32_t(252)),singleBits(scalar(f[0]))))return {};c.pc=0x8000fc50;break;} // d01e00fc stfs 0, 252(30)
case 0x8000fc50:{if(!store((r[30]+uint32_t(256)),singleBits(scalar(f[0]))))return {};c.pc=0x8000fc54;break;} // d01e0100 stfs 0, 256(30)
case 0x8000fc54:{c.lr=0x8000fc58;c.pc=0x8002eb30;break;} // 4801eedd bl 0x8002eb30
case 0x8000fc58:{if(!half((r[3]+uint32_t(4)),r[3]))return {};c.pc=0x8000fc5c;break;} // a0630004 lhz 3, 4(3)
case 0x8000fc5c:{r[0]=uint32_t(17200)<<16;c.pc=0x8000fc60;break;} // 3c004330 lis 0, 17200
case 0x8000fc60:{if(!store((r[1]+uint32_t(8)),r[0]))return {};c.pc=0x8000fc64;break;} // 90010008 stw 0, 8(1)
case 0x8000fc64:{if(!c.step){if(!word((r[31]+uint32_t(104)),c.transfer))return {};c.step=1;}uint32_t low;if(!word((r[31]+uint32_t(104))+4,low))return {};f[1]=(uint64_t(c.transfer)<<32)|low;c.step=0;c.pc=0x8000fc68;break;} // c83f0068 lfd 1, 104(31)
case 0x8000fc68:{if(!store((r[1]+uint32_t(12)),r[3]))return {};c.pc=0x8000fc6c;break;} // 9061000c stw 3, 12(1)
case 0x8000fc6c:{if(!c.step){if(!word((r[1]+uint32_t(8)),c.transfer))return {};c.step=1;}uint32_t low;if(!word((r[1]+uint32_t(8))+4,low))return {};f[0]=(uint64_t(c.transfer)<<32)|low;c.step=0;c.pc=0x8000fc70;break;} // c8010008 lfd 0, 8(1)
case 0x8000fc70:{f[0]=rounded(scalar(f[0])-scalar(f[1]));c.pc=0x8000fc74;break;} // ec000828 fsubs 0, 0, 1
case 0x8000fc74:{if(!store((r[30]+uint32_t(260)),singleBits(scalar(f[0]))))return {};c.pc=0x8000fc78;break;} // d01e0104 stfs 0, 260(30)
case 0x8000fc78:{c.lr=0x8000fc7c;c.pc=0x8002eb30;break;} // 4801eeb9 bl 0x8002eb30
case 0x8000fc7c:{if(!half((r[3]+uint32_t(6)),r[4]))return {};c.pc=0x8000fc80;break;} // a0830006 lhz 4, 6(3)
case 0x8000fc80:{r[0]=uint32_t(17200)<<16;c.pc=0x8000fc84;break;} // 3c004330 lis 0, 17200
case 0x8000fc84:{if(!store((r[1]+uint32_t(16)),r[0]))return {};c.pc=0x8000fc88;break;} // 90010010 stw 0, 16(1)
case 0x8000fc88:{r[0]=uint32_t(0);c.pc=0x8000fc8c;break;} // 38000000 li 0, 0
case 0x8000fc8c:{if(!c.step){if(!word((r[31]+uint32_t(104)),c.transfer))return {};c.step=1;}uint32_t low;if(!word((r[31]+uint32_t(104))+4,low))return {};f[3]=(uint64_t(c.transfer)<<32)|low;c.step=0;c.pc=0x8000fc90;break;} // c87f0068 lfd 3, 104(31)
case 0x8000fc90:{r[3]=r[30];c.pc=0x8000fc94;break;} // 7fc3f378 mr 3, 30
case 0x8000fc94:{if(!store((r[1]+uint32_t(20)),r[4]))return {};c.pc=0x8000fc98;break;} // 90810014 stw 4, 20(1)
case 0x8000fc98:{uint32_t v;if(!word((r[2]+uint32_t(-32140)),v))return {};f[2]=bits(double(single(v)));c.paired[2]=v;c.pc=0x8000fc9c;break;} // c0428274 lfs 2, -32140(2)
case 0x8000fc9c:{if(!c.step){if(!word((r[1]+uint32_t(16)),c.transfer))return {};c.step=1;}uint32_t low;if(!word((r[1]+uint32_t(16))+4,low))return {};f[0]=(uint64_t(c.transfer)<<32)|low;c.step=0;c.pc=0x8000fca0;break;} // c8010010 lfd 0, 16(1)
case 0x8000fca0:{uint32_t v;if(!word((r[2]+uint32_t(-32068)),v))return {};f[1]=bits(double(single(v)));c.paired[1]=v;c.pc=0x8000fca4;break;} // c02282bc lfs 1, -32068(2)
case 0x8000fca4:{f[3]=rounded(scalar(f[0])-scalar(f[3]));c.pc=0x8000fca8;break;} // ec601828 fsubs 3, 0, 3
case 0x8000fca8:{uint32_t v;if(!word((r[2]+uint32_t(-32064)),v))return {};f[0]=bits(double(single(v)));c.paired[0]=v;c.pc=0x8000fcac;break;} // c00282c0 lfs 0, -32064(2)
case 0x8000fcac:{if(!store((r[30]+uint32_t(264)),singleBits(scalar(f[3]))))return {};c.pc=0x8000fcb0;break;} // d07e0108 stfs 3, 264(30)
case 0x8000fcb0:{if(!store((r[30]+uint32_t(268)),singleBits(scalar(f[30]))))return {};c.pc=0x8000fcb4;break;} // d3de010c stfs 30, 268(30)
case 0x8000fcb4:{if(!store((r[30]+uint32_t(272)),singleBits(scalar(f[31]))))return {};c.pc=0x8000fcb8;break;} // d3fe0110 stfs 31, 272(30)
case 0x8000fcb8:{if(!store((r[30]+uint32_t(496)),singleBits(scalar(f[2]))))return {};c.pc=0x8000fcbc;break;} // d05e01f0 stfs 2, 496(30)
case 0x8000fcbc:{if(!store((r[30]+uint32_t(500)),singleBits(scalar(f[1]))))return {};c.pc=0x8000fcc0;break;} // d03e01f4 stfs 1, 500(30)
case 0x8000fcc0:{if(!store((r[30]+uint32_t(504)),singleBits(scalar(f[0]))))return {};c.pc=0x8000fcc4;break;} // d01e01f8 stfs 0, 504(30)
case 0x8000fcc4:{if(!store((r[30]+uint32_t(524)),singleBits(scalar(f[2]))))return {};c.pc=0x8000fcc8;break;} // d05e020c stfs 2, 524(30)
case 0x8000fcc8:{if(!store((r[30]+uint32_t(528)),singleBits(scalar(f[2]))))return {};c.pc=0x8000fccc;break;} // d05e0210 stfs 2, 528(30)
case 0x8000fccc:{if(!store((r[30]+uint32_t(532)),singleBits(scalar(f[2]))))return {};c.pc=0x8000fcd0;break;} // d05e0214 stfs 2, 532(30)
case 0x8000fcd0:{if(!store((r[30]+uint32_t(512)),singleBits(scalar(f[2]))))return {};c.pc=0x8000fcd4;break;} // d05e0200 stfs 2, 512(30)
case 0x8000fcd4:{if(!store((r[30]+uint32_t(516)),singleBits(scalar(f[2]))))return {};c.pc=0x8000fcd8;break;} // d05e0204 stfs 2, 516(30)
case 0x8000fcd8:{if(!store((r[30]+uint32_t(520)),singleBits(scalar(f[2]))))return {};c.pc=0x8000fcdc;break;} // d05e0208 stfs 2, 520(30)
case 0x8000fcdc:{if(!s.storeHalf||!s.storeHalf(s.user,(r[30]+uint32_t(536)),uint16_t(r[0])))return {};c.pc=0x8000fce0;break;} // b01e0218 sth 0, 536(30)
case 0x8000fce0:{if(!s.storeHalf||!s.storeHalf(s.user,(r[30]+uint32_t(538)),uint16_t(r[0])))return {};c.pc=0x8000fce4;break;} // b01e021a sth 0, 538(30)
case 0x8000fce4:{if(!s.storeHalf||!s.storeHalf(s.user,(r[30]+uint32_t(540)),uint16_t(r[0])))return {};c.pc=0x8000fce8;break;} // b01e021c sth 0, 540(30)
case 0x8000fce8:{if(!s.storeHalf||!s.storeHalf(s.user,(r[30]+uint32_t(542)),uint16_t(r[0])))return {};c.pc=0x8000fcec;break;} // b01e021e sth 0, 542(30)
case 0x8000fcec:{if(!word((r[31]+uint32_t(0)),r[4]))return {};c.pc=0x8000fcf0;break;} // 809f0000 lwz 4, 0(31)
case 0x8000fcf0:{if(!word((r[31]+uint32_t(4)),r[0]))return {};c.pc=0x8000fcf4;break;} // 801f0004 lwz 0, 4(31)
case 0x8000fcf4:{if(!store((r[30]+uint32_t(548)),r[4]))return {};c.pc=0x8000fcf8;break;} // 909e0224 stw 4, 548(30)
case 0x8000fcf8:{if(!store((r[30]+uint32_t(552)),r[0]))return {};c.pc=0x8000fcfc;break;} // 901e0228 stw 0, 552(30)
case 0x8000fcfc:{if(!word((r[31]+uint32_t(8)),r[0]))return {};c.pc=0x8000fd00;break;} // 801f0008 lwz 0, 8(31)
case 0x8000fd00:{if(!store((r[30]+uint32_t(556)),r[0]))return {};c.pc=0x8000fd04;break;} // 901e022c stw 0, 556(30)
case 0x8000fd04:{if(!word((r[31]+uint32_t(12)),r[4]))return {};c.pc=0x8000fd08;break;} // 809f000c lwz 4, 12(31)
case 0x8000fd08:{if(!word((r[31]+uint32_t(16)),r[0]))return {};c.pc=0x8000fd0c;break;} // 801f0010 lwz 0, 16(31)
case 0x8000fd0c:{if(!store((r[30]+uint32_t(560)),r[4]))return {};c.pc=0x8000fd10;break;} // 909e0230 stw 4, 560(30)
case 0x8000fd10:{if(!store((r[30]+uint32_t(564)),r[0]))return {};c.pc=0x8000fd14;break;} // 901e0234 stw 0, 564(30)
case 0x8000fd14:{if(!word((r[31]+uint32_t(20)),r[0]))return {};c.pc=0x8000fd18;break;} // 801f0014 lwz 0, 20(31)
case 0x8000fd18:{if(!store((r[30]+uint32_t(568)),r[0]))return {};c.pc=0x8000fd1c;break;} // 901e0238 stw 0, 568(30)
case 0x8000fd1c:{if(!word((r[31]+uint32_t(24)),r[4]))return {};c.pc=0x8000fd20;break;} // 809f0018 lwz 4, 24(31)
case 0x8000fd20:{if(!word((r[31]+uint32_t(28)),r[0]))return {};c.pc=0x8000fd24;break;} // 801f001c lwz 0, 28(31)
case 0x8000fd24:{if(!store((r[30]+uint32_t(572)),r[4]))return {};c.pc=0x8000fd28;break;} // 909e023c stw 4, 572(30)
case 0x8000fd28:{if(!store((r[30]+uint32_t(576)),r[0]))return {};c.pc=0x8000fd2c;break;} // 901e0240 stw 0, 576(30)
case 0x8000fd2c:{if(!word((r[31]+uint32_t(32)),r[0]))return {};c.pc=0x8000fd30;break;} // 801f0020 lwz 0, 32(31)
case 0x8000fd30:{if(!store((r[30]+uint32_t(580)),r[0]))return {};c.pc=0x8000fd34;break;} // 901e0244 stw 0, 580(30)
case 0x8000fd34:{if(!word((r[31]+uint32_t(36)),r[4]))return {};c.pc=0x8000fd38;break;} // 809f0024 lwz 4, 36(31)
case 0x8000fd38:{if(!word((r[31]+uint32_t(40)),r[0]))return {};c.pc=0x8000fd3c;break;} // 801f0028 lwz 0, 40(31)
case 0x8000fd3c:{if(!store((r[30]+uint32_t(584)),r[4]))return {};c.pc=0x8000fd40;break;} // 909e0248 stw 4, 584(30)
case 0x8000fd40:{if(!store((r[30]+uint32_t(588)),r[0]))return {};c.pc=0x8000fd44;break;} // 901e024c stw 0, 588(30)
case 0x8000fd44:{if(!word((r[31]+uint32_t(44)),r[0]))return {};c.pc=0x8000fd48;break;} // 801f002c lwz 0, 44(31)
case 0x8000fd48:{if(!store((r[30]+uint32_t(592)),r[0]))return {};c.pc=0x8000fd4c;break;} // 901e0250 stw 0, 592(30)
case 0x8000fd4c:{if(!c.step){uint32_t v;if(!word((r[1]+uint32_t(136)),v))return {};f[31]=bits(double(single(v)));c.step=1;}if(!word((r[1]+uint32_t(136))+4,c.paired[31]))return {};c.step=0;c.pc=0x8000fd50;break;} // e3e10088 psq_l f31, 136(r1), 0, 0
case 0x8000fd50:{if(!c.step){if(!word((r[1]+uint32_t(128)),c.transfer))return {};c.step=1;}uint32_t low;if(!word((r[1]+uint32_t(128))+4,low))return {};f[31]=(uint64_t(c.transfer)<<32)|low;c.step=0;c.pc=0x8000fd54;break;} // cbe10080 lfd 31, 128(1)
case 0x8000fd54:{if(!c.step){uint32_t v;if(!word((r[1]+uint32_t(120)),v))return {};f[30]=bits(double(single(v)));c.step=1;}if(!word((r[1]+uint32_t(120))+4,c.paired[30]))return {};c.step=0;c.pc=0x8000fd58;break;} // e3c10078 psq_l f30, 120(r1), 0, 0
case 0x8000fd58:{if(!c.step){if(!word((r[1]+uint32_t(112)),c.transfer))return {};c.step=1;}uint32_t low;if(!word((r[1]+uint32_t(112))+4,low))return {};f[30]=(uint64_t(c.transfer)<<32)|low;c.step=0;c.pc=0x8000fd5c;break;} // cbc10070 lfd 30, 112(1)
case 0x8000fd5c:{if(!c.step){uint32_t v;if(!word((r[1]+uint32_t(104)),v))return {};f[29]=bits(double(single(v)));c.step=1;}if(!word((r[1]+uint32_t(104))+4,c.paired[29]))return {};c.step=0;c.pc=0x8000fd60;break;} // e3a10068 psq_l f29, 104(r1), 0, 0
case 0x8000fd60:{if(!c.step){if(!word((r[1]+uint32_t(96)),c.transfer))return {};c.step=1;}uint32_t low;if(!word((r[1]+uint32_t(96))+4,low))return {};f[29]=(uint64_t(c.transfer)<<32)|low;c.step=0;c.pc=0x8000fd64;break;} // cba10060 lfd 29, 96(1)
case 0x8000fd64:{if(!c.step){uint32_t v;if(!word((r[1]+uint32_t(88)),v))return {};f[28]=bits(double(single(v)));c.step=1;}if(!word((r[1]+uint32_t(88))+4,c.paired[28]))return {};c.step=0;c.pc=0x8000fd68;break;} // e3810058 psq_l f28, 88(r1), 0, 0
case 0x8000fd68:{if(!c.step){if(!word((r[1]+uint32_t(80)),c.transfer))return {};c.step=1;}uint32_t low;if(!word((r[1]+uint32_t(80))+4,low))return {};f[28]=(uint64_t(c.transfer)<<32)|low;c.step=0;c.pc=0x8000fd6c;break;} // cb810050 lfd 28, 80(1)
case 0x8000fd6c:{if(!c.step){uint32_t v;if(!word((r[1]+uint32_t(72)),v))return {};f[27]=bits(double(single(v)));c.step=1;}if(!word((r[1]+uint32_t(72))+4,c.paired[27]))return {};c.step=0;c.pc=0x8000fd70;break;} // e3610048 psq_l f27, 72(r1), 0, 0
case 0x8000fd70:{if(!c.step){if(!word((r[1]+uint32_t(64)),c.transfer))return {};c.step=1;}uint32_t low;if(!word((r[1]+uint32_t(64))+4,low))return {};f[27]=(uint64_t(c.transfer)<<32)|low;c.step=0;c.pc=0x8000fd74;break;} // cb610040 lfd 27, 64(1)
case 0x8000fd74:{if(!c.step){uint32_t v;if(!word((r[1]+uint32_t(56)),v))return {};f[26]=bits(double(single(v)));c.step=1;}if(!word((r[1]+uint32_t(56))+4,c.paired[26]))return {};c.step=0;c.pc=0x8000fd78;break;} // e3410038 psq_l f26, 56(r1), 0, 0
case 0x8000fd78:{if(!c.step){if(!word((r[1]+uint32_t(48)),c.transfer))return {};c.step=1;}uint32_t low;if(!word((r[1]+uint32_t(48))+4,low))return {};f[26]=(uint64_t(c.transfer)<<32)|low;c.step=0;c.pc=0x8000fd7c;break;} // cb410030 lfd 26, 48(1)
case 0x8000fd7c:{if(!word((r[1]+uint32_t(44)),r[31]))return {};c.pc=0x8000fd80;break;} // 83e1002c lwz 31, 44(1)
case 0x8000fd80:{if(!word((r[1]+uint32_t(40)),r[30]))return {};c.pc=0x8000fd84;break;} // 83c10028 lwz 30, 40(1)
case 0x8000fd84:{if(!word((r[1]+uint32_t(36)),r[29]))return {};c.pc=0x8000fd88;break;} // 83a10024 lwz 29, 36(1)
case 0x8000fd88:{if(!word((r[1]+uint32_t(148)),r[0]))return {};c.pc=0x8000fd8c;break;} // 80010094 lwz 0, 148(1)
case 0x8000fd8c:{if(!word((r[1]+uint32_t(32)),r[28]))return {};c.pc=0x8000fd90;break;} // 83810020 lwz 28, 32(1)
case 0x8000fd90:{c.lr=r[0];c.pc=0x8000fd94;break;} // 7c0803a6 mtlr 0
case 0x8000fd94:{r[1]=r[1]+uint32_t(144);c.pc=0x8000fd98;break;} // 38210090 addi 1, 1, 144
case 0x8000fd98:{c.pc=c.lr;break;} // 4e800020 blr 
case 0x8029e7e8:{r[0]=c.lr;c.pc=0x8029e7ec;break;} // 7c0802a6 mflr 0
case 0x8029e7ec:{if(!store((r[1]+uint32_t(4)),r[0]))return {};c.pc=0x8029e7f0;break;} // 90010004 stw 0, 4(1)
case 0x8029e7f0:{const auto target=(r[1]+uint32_t(-64));if(!store(target,r[1]))return {};r[1]=target;c.pc=0x8029e7f4;break;} // 9421ffc0 stwu 1, -64(1)
case 0x8029e7f4:{while(c.step<2){if(!store((r[1]+uint32_t(56))+4*c.step,uint32_t(f[31]>>(c.step?0:32))))return {};++c.step;}c.step=0;c.pc=0x8029e7f8;break;} // dbe10038 stfd 31, 56(1)
case 0x8029e7f8:{f[31]=f[4];c.pc=0x8029e7fc;break;} // ffe02090 fmr 31, 4
case 0x8029e7fc:{while(c.step<2){if(!store((r[1]+uint32_t(48))+4*c.step,uint32_t(f[30]>>(c.step?0:32))))return {};++c.step;}c.step=0;c.pc=0x8029e800;break;} // dbc10030 stfd 30, 48(1)
case 0x8029e800:{f[30]=f[3];c.pc=0x8029e804;break;} // ffc01890 fmr 30, 3
case 0x8029e804:{while(c.step<2){if(!store((r[1]+uint32_t(40))+4*c.step,uint32_t(f[29]>>(c.step?0:32))))return {};++c.step;}c.step=0;c.pc=0x8029e808;break;} // dba10028 stfd 29, 40(1)
case 0x8029e808:{f[29]=f[2];c.pc=0x8029e80c;break;} // ffa01090 fmr 29, 2
case 0x8029e80c:{if(!store((r[1]+uint32_t(36)),r[31]))return {};c.pc=0x8029e810;break;} // 93e10024 stw 31, 36(1)
case 0x8029e810:{r[31]=r[3];c.pc=0x8029e814;break;} // 7c7f1b78 mr 31, 3
case 0x8029e814:{uint32_t v;if(!word((r[2]+uint32_t(8008)),v))return {};f[5]=bits(double(single(v)));c.paired[5]=v;c.pc=0x8029e818;break;} // c0a21f48 lfs 5, 8008(2)
case 0x8029e818:{uint32_t v;if(!word((r[2]+uint32_t(8012)),v))return {};f[0]=bits(double(single(v)));c.paired[0]=v;c.pc=0x8029e81c;break;} // c0021f4c lfs 0, 8012(2)
case 0x8029e81c:{f[1]=rounded(scalar(f[5])*scalar(f[1]));c.pc=0x8029e820;break;} // ec250072 fmuls 1, 5, 1
case 0x8029e820:{f[1]=rounded(scalar(f[0])*scalar(f[1]));c.pc=0x8029e824;break;} // ec200072 fmuls 1, 0, 1
case 0x8029e824:{if(!external(0x80272864,0x8029e828))return {};break;} // 4bfd4041 bl 0x80272864
case 0x8029e828:{uint32_t v;if(!word((r[2]+uint32_t(7992)),v))return {};f[3]=bits(double(single(v)));c.paired[3]=v;c.pc=0x8029e82c;break;} // c0621f38 lfs 3, 7992(2)
case 0x8029e82c:{f[2]=rounded(scalar(f[31])-scalar(f[30]));c.pc=0x8029e830;break;} // ec5ff028 fsubs 2, 31, 30
case 0x8029e830:{f[0]=rounded(scalar(f[31])*scalar(f[30]));c.pc=0x8029e834;break;} // ec1f07b2 fmuls 0, 31, 30
case 0x8029e834:{f[4]=rounded(scalar(f[3])/scalar(f[1]));c.pc=0x8029e838;break;} // ec830824 fdivs 4, 3, 1
case 0x8029e838:{f[1]=rounded(scalar(f[4])/scalar(f[29]));c.pc=0x8029e83c;break;} // ec24e824 fdivs 1, 4, 29
case 0x8029e83c:{if(!store((r[31]+uint32_t(0)),singleBits(scalar(f[1]))))return {};c.pc=0x8029e840;break;} // d03f0000 stfs 1, 0(31)
case 0x8029e840:{f[3]=rounded(scalar(f[3])/scalar(f[2]));c.pc=0x8029e844;break;} // ec631024 fdivs 3, 3, 2
case 0x8029e844:{uint32_t v;if(!word((r[2]+uint32_t(8000)),v))return {};f[2]=bits(double(single(v)));c.paired[2]=v;c.pc=0x8029e848;break;} // c0421f40 lfs 2, 8000(2)
case 0x8029e848:{if(!store((r[31]+uint32_t(4)),singleBits(scalar(f[2]))))return {};c.pc=0x8029e84c;break;} // d05f0004 stfs 2, 4(31)
case 0x8029e84c:{if(!store((r[31]+uint32_t(8)),singleBits(scalar(f[2]))))return {};c.pc=0x8029e850;break;} // d05f0008 stfs 2, 8(31)
case 0x8029e850:{if(!store((r[31]+uint32_t(12)),singleBits(scalar(f[2]))))return {};c.pc=0x8029e854;break;} // d05f000c stfs 2, 12(31)
case 0x8029e854:{if(!store((r[31]+uint32_t(16)),singleBits(scalar(f[2]))))return {};c.pc=0x8029e858;break;} // d05f0010 stfs 2, 16(31)
case 0x8029e858:{f[1]=f[30]^0x8000000000000000ull;c.pc=0x8029e85c;break;} // fc20f050 fneg 1, 30
case 0x8029e85c:{f[0]=f[0]^0x8000000000000000ull;c.pc=0x8029e860;break;} // fc000050 fneg 0, 0
case 0x8029e860:{if(!store((r[31]+uint32_t(20)),singleBits(scalar(f[4]))))return {};c.pc=0x8029e864;break;} // d09f0014 stfs 4, 20(31)
case 0x8029e864:{f[1]=rounded(scalar(f[1])*scalar(f[3]));c.pc=0x8029e868;break;} // ec2100f2 fmuls 1, 1, 3
case 0x8029e868:{if(!store((r[31]+uint32_t(24)),singleBits(scalar(f[2]))))return {};c.pc=0x8029e86c;break;} // d05f0018 stfs 2, 24(31)
case 0x8029e86c:{f[0]=rounded(scalar(f[3])*scalar(f[0]));c.pc=0x8029e870;break;} // ec030032 fmuls 0, 3, 0
case 0x8029e870:{if(!store((r[31]+uint32_t(28)),singleBits(scalar(f[2]))))return {};c.pc=0x8029e874;break;} // d05f001c stfs 2, 28(31)
case 0x8029e874:{if(!store((r[31]+uint32_t(32)),singleBits(scalar(f[2]))))return {};c.pc=0x8029e878;break;} // d05f0020 stfs 2, 32(31)
case 0x8029e878:{if(!store((r[31]+uint32_t(36)),singleBits(scalar(f[2]))))return {};c.pc=0x8029e87c;break;} // d05f0024 stfs 2, 36(31)
case 0x8029e87c:{if(!store((r[31]+uint32_t(40)),singleBits(scalar(f[1]))))return {};c.pc=0x8029e880;break;} // d03f0028 stfs 1, 40(31)
case 0x8029e880:{if(!store((r[31]+uint32_t(44)),singleBits(scalar(f[0]))))return {};c.pc=0x8029e884;break;} // d01f002c stfs 0, 44(31)
case 0x8029e884:{if(!store((r[31]+uint32_t(48)),singleBits(scalar(f[2]))))return {};c.pc=0x8029e888;break;} // d05f0030 stfs 2, 48(31)
case 0x8029e888:{if(!store((r[31]+uint32_t(52)),singleBits(scalar(f[2]))))return {};c.pc=0x8029e88c;break;} // d05f0034 stfs 2, 52(31)
case 0x8029e88c:{uint32_t v;if(!word((r[2]+uint32_t(8004)),v))return {};f[0]=bits(double(single(v)));c.paired[0]=v;c.pc=0x8029e890;break;} // c0021f44 lfs 0, 8004(2)
case 0x8029e890:{if(!store((r[31]+uint32_t(56)),singleBits(scalar(f[0]))))return {};c.pc=0x8029e894;break;} // d01f0038 stfs 0, 56(31)
case 0x8029e894:{if(!store((r[31]+uint32_t(60)),singleBits(scalar(f[2]))))return {};c.pc=0x8029e898;break;} // d05f003c stfs 2, 60(31)
case 0x8029e898:{if(!word((r[1]+uint32_t(68)),r[0]))return {};c.pc=0x8029e89c;break;} // 80010044 lwz 0, 68(1)
case 0x8029e89c:{if(!c.step){if(!word((r[1]+uint32_t(56)),c.transfer))return {};c.step=1;}uint32_t low;if(!word((r[1]+uint32_t(56))+4,low))return {};f[31]=(uint64_t(c.transfer)<<32)|low;c.step=0;c.pc=0x8029e8a0;break;} // cbe10038 lfd 31, 56(1)
case 0x8029e8a0:{if(!c.step){if(!word((r[1]+uint32_t(48)),c.transfer))return {};c.step=1;}uint32_t low;if(!word((r[1]+uint32_t(48))+4,low))return {};f[30]=(uint64_t(c.transfer)<<32)|low;c.step=0;c.pc=0x8029e8a4;break;} // cbc10030 lfd 30, 48(1)
case 0x8029e8a4:{if(!c.step){if(!word((r[1]+uint32_t(40)),c.transfer))return {};c.step=1;}uint32_t low;if(!word((r[1]+uint32_t(40))+4,low))return {};f[29]=(uint64_t(c.transfer)<<32)|low;c.step=0;c.pc=0x8029e8a8;break;} // cba10028 lfd 29, 40(1)
case 0x8029e8a8:{if(!word((r[1]+uint32_t(36)),r[31]))return {};c.pc=0x8029e8ac;break;} // 83e10024 lwz 31, 36(1)
case 0x8029e8ac:{r[1]=r[1]+uint32_t(64);c.pc=0x8029e8b0;break;} // 38210040 addi 1, 1, 64
case 0x8029e8b0:{c.lr=r[0];c.pc=0x8029e8b4;break;} // 7c0803a6 mtlr 0
case 0x8029e8b4:{c.pc=c.lr;break;} // 4e800020 blr 
case 0x8029e8b8:{f[8]=rounded(scalar(f[4])-scalar(f[3]));c.pc=0x8029e8bc;break;} // ed041828 fsubs 8, 4, 3
case 0x8029e8bc:{uint32_t v;if(!word((r[2]+uint32_t(7992)),v))return {};f[9]=bits(double(single(v)));c.paired[9]=v;c.pc=0x8029e8c0;break;} // c1221f38 lfs 9, 7992(2)
case 0x8029e8c0:{f[0]=rounded(scalar(f[1])-scalar(f[2]));c.pc=0x8029e8c4;break;} // ec011028 fsubs 0, 1, 2
case 0x8029e8c4:{uint32_t v;if(!word((r[2]+uint32_t(7996)),v))return {};f[7]=bits(double(single(v)));c.paired[7]=v;c.pc=0x8029e8c8;break;} // c0e21f3c lfs 7, 7996(2)
case 0x8029e8c8:{f[3]=rounded(scalar(f[4])+scalar(f[3]));c.pc=0x8029e8cc;break;} // ec64182a fadds 3, 4, 3
case 0x8029e8cc:{f[10]=rounded(scalar(f[9])/scalar(f[8]));c.pc=0x8029e8d0;break;} // ed494024 fdivs 10, 9, 8
case 0x8029e8d0:{f[8]=rounded(scalar(f[9])/scalar(f[0]));c.pc=0x8029e8d4;break;} // ed090024 fdivs 8, 9, 0
case 0x8029e8d4:{f[4]=rounded(scalar(f[7])*scalar(f[10]));c.pc=0x8029e8d8;break;} // ec8702b2 fmuls 4, 7, 10
case 0x8029e8d8:{f[3]=f[3]^0x8000000000000000ull;c.pc=0x8029e8dc;break;} // fc601850 fneg 3, 3
case 0x8029e8dc:{f[0]=rounded(scalar(f[6])-scalar(f[5]));c.pc=0x8029e8e0;break;} // ec062828 fsubs 0, 6, 5
case 0x8029e8e0:{if(!store((r[3]+uint32_t(0)),singleBits(scalar(f[4]))))return {};c.pc=0x8029e8e4;break;} // d0830000 stfs 4, 0(3)
case 0x8029e8e4:{f[1]=rounded(scalar(f[1])+scalar(f[2]));c.pc=0x8029e8e8;break;} // ec21102a fadds 1, 1, 2
case 0x8029e8e8:{f[2]=rounded(scalar(f[10])*scalar(f[3]));c.pc=0x8029e8ec;break;} // ec4a00f2 fmuls 2, 10, 3
case 0x8029e8ec:{uint32_t v;if(!word((r[2]+uint32_t(8000)),v))return {};f[3]=bits(double(single(v)));c.paired[3]=v;c.pc=0x8029e8f0;break;} // c0621f40 lfs 3, 8000(2)
case 0x8029e8f0:{f[1]=f[1]^0x8000000000000000ull;c.pc=0x8029e8f4;break;} // fc200850 fneg 1, 1
case 0x8029e8f4:{if(!store((r[3]+uint32_t(4)),singleBits(scalar(f[3]))))return {};c.pc=0x8029e8f8;break;} // d0630004 stfs 3, 4(3)
case 0x8029e8f8:{f[4]=rounded(scalar(f[9])/scalar(f[0]));c.pc=0x8029e8fc;break;} // ec890024 fdivs 4, 9, 0
case 0x8029e8fc:{if(!store((r[3]+uint32_t(8)),singleBits(scalar(f[3]))))return {};c.pc=0x8029e900;break;} // d0630008 stfs 3, 8(3)
case 0x8029e900:{if(!store((r[3]+uint32_t(12)),singleBits(scalar(f[2]))))return {};c.pc=0x8029e904;break;} // d043000c stfs 2, 12(3)
case 0x8029e904:{if(!store((r[3]+uint32_t(16)),singleBits(scalar(f[3]))))return {};c.pc=0x8029e908;break;} // d0630010 stfs 3, 16(3)
case 0x8029e908:{f[2]=rounded(scalar(f[7])*scalar(f[8]));c.pc=0x8029e90c;break;} // ec470232 fmuls 2, 7, 8
case 0x8029e90c:{f[0]=f[6]^0x8000000000000000ull;c.pc=0x8029e910;break;} // fc003050 fneg 0, 6
case 0x8029e910:{f[1]=rounded(scalar(f[8])*scalar(f[1]));c.pc=0x8029e914;break;} // ec280072 fmuls 1, 8, 1
case 0x8029e914:{if(!store((r[3]+uint32_t(20)),singleBits(scalar(f[2]))))return {};c.pc=0x8029e918;break;} // d0430014 stfs 2, 20(3)
case 0x8029e918:{f[0]=rounded(scalar(f[0])*scalar(f[4]));c.pc=0x8029e91c;break;} // ec000132 fmuls 0, 0, 4
case 0x8029e91c:{if(!store((r[3]+uint32_t(24)),singleBits(scalar(f[3]))))return {};c.pc=0x8029e920;break;} // d0630018 stfs 3, 24(3)
case 0x8029e920:{if(!store((r[3]+uint32_t(28)),singleBits(scalar(f[1]))))return {};c.pc=0x8029e924;break;} // d023001c stfs 1, 28(3)
case 0x8029e924:{if(!store((r[3]+uint32_t(32)),singleBits(scalar(f[3]))))return {};c.pc=0x8029e928;break;} // d0630020 stfs 3, 32(3)
case 0x8029e928:{if(!store((r[3]+uint32_t(36)),singleBits(scalar(f[3]))))return {};c.pc=0x8029e92c;break;} // d0630024 stfs 3, 36(3)
case 0x8029e92c:{uint32_t v;if(!word((r[2]+uint32_t(8004)),v))return {};f[1]=bits(double(single(v)));c.paired[1]=v;c.pc=0x8029e930;break;} // c0221f44 lfs 1, 8004(2)
case 0x8029e930:{f[1]=rounded(scalar(f[1])*scalar(f[4]));c.pc=0x8029e934;break;} // ec210132 fmuls 1, 1, 4
case 0x8029e934:{if(!store((r[3]+uint32_t(40)),singleBits(scalar(f[1]))))return {};c.pc=0x8029e938;break;} // d0230028 stfs 1, 40(3)
case 0x8029e938:{if(!store((r[3]+uint32_t(44)),singleBits(scalar(f[0]))))return {};c.pc=0x8029e93c;break;} // d003002c stfs 0, 44(3)
case 0x8029e93c:{if(!store((r[3]+uint32_t(48)),singleBits(scalar(f[3]))))return {};c.pc=0x8029e940;break;} // d0630030 stfs 3, 48(3)
case 0x8029e940:{if(!store((r[3]+uint32_t(52)),singleBits(scalar(f[3]))))return {};c.pc=0x8029e944;break;} // d0630034 stfs 3, 52(3)
case 0x8029e944:{if(!store((r[3]+uint32_t(56)),singleBits(scalar(f[3]))))return {};c.pc=0x8029e948;break;} // d0630038 stfs 3, 56(3)
case 0x8029e948:{if(!store((r[3]+uint32_t(60)),singleBits(scalar(f[9]))))return {};c.pc=0x8029e94c;break;} // d123003c stfs 9, 60(3)
case 0x8029e94c:{c.pc=c.lr;break;} // 4e800020 blr 
case 0x8002eb30:{if(!word((r[13]+uint32_t(6292)),r[3]))return {};c.pc=0x8002eb34;break;} // 806d1894 lwz 3, 6292(13)
case 0x8002eb34:{c.pc=c.lr;break;} // 4e800020 blr 
case 0x8002ed34:{if(!word((r[31]+uint32_t(0)),r[3]))return {};c.pc=0x8002ed38;break;} // 807f0000 lwz 3, 0(31)
case 0x8002ed38:{r[4]=uint32_t(-32707)<<16;c.pc=0x8002ed3c;break;} // 3c80803d lis 4, -32707
case 0x8002ed3c:{if(!word((r[31]+uint32_t(4)),r[0]))return {};c.pc=0x8002ed40;break;} // 801f0004 lwz 0, 4(31)
case 0x8002ed40:{const auto target=(r[4]+uint32_t(-24744));if(!store(target,r[3]))return {};r[4]=target;c.pc=0x8002ed44;break;} // 94649f58 stwu 3, -24744(4)
case 0x8002ed44:{if(!store((r[4]+uint32_t(4)),r[0]))return {};c.pc=0x8002ed48;break;} // 90040004 stw 0, 4(4)
case 0x8002ed48:{if(!word((r[31]+uint32_t(8)),r[3]))return {};c.pc=0x8002ed4c;break;} // 807f0008 lwz 3, 8(31)
case 0x8002ed4c:{if(!word((r[31]+uint32_t(12)),r[0]))return {};c.pc=0x8002ed50;break;} // 801f000c lwz 0, 12(31)
case 0x8002ed50:{if(!store((r[4]+uint32_t(8)),r[3]))return {};c.pc=0x8002ed54;break;} // 90640008 stw 3, 8(4)
case 0x8002ed54:{if(!store((r[4]+uint32_t(12)),r[0]))return {};c.pc=0x8002ed58;break;} // 9004000c stw 0, 12(4)
case 0x8002ed58:{if(!word((r[31]+uint32_t(16)),r[3]))return {};c.pc=0x8002ed5c;break;} // 807f0010 lwz 3, 16(31)
case 0x8002ed5c:{if(!word((r[31]+uint32_t(20)),r[0]))return {};c.pc=0x8002ed60;break;} // 801f0014 lwz 0, 20(31)
case 0x8002ed60:{if(!store((r[4]+uint32_t(16)),r[3]))return {};c.pc=0x8002ed64;break;} // 90640010 stw 3, 16(4)
case 0x8002ed64:{if(!store((r[4]+uint32_t(20)),r[0]))return {};c.pc=0x8002ed68;break;} // 90040014 stw 0, 20(4)
case 0x8002ed68:{if(!word((r[31]+uint32_t(24)),r[3]))return {};c.pc=0x8002ed6c;break;} // 807f0018 lwz 3, 24(31)
case 0x8002ed6c:{if(!word((r[31]+uint32_t(28)),r[0]))return {};c.pc=0x8002ed70;break;} // 801f001c lwz 0, 28(31)
case 0x8002ed70:{if(!store((r[4]+uint32_t(24)),r[3]))return {};c.pc=0x8002ed74;break;} // 90640018 stw 3, 24(4)
case 0x8002ed74:{if(!store((r[4]+uint32_t(28)),r[0]))return {};c.pc=0x8002ed78;break;} // 9004001c stw 0, 28(4)
case 0x8002ed78:{if(!word((r[31]+uint32_t(32)),r[3]))return {};c.pc=0x8002ed7c;break;} // 807f0020 lwz 3, 32(31)
case 0x8002ed7c:{if(!word((r[31]+uint32_t(36)),r[0]))return {};c.pc=0x8002ed80;break;} // 801f0024 lwz 0, 36(31)
case 0x8002ed80:{if(!store((r[4]+uint32_t(32)),r[3]))return {};c.pc=0x8002ed84;break;} // 90640020 stw 3, 32(4)
case 0x8002ed84:{if(!store((r[4]+uint32_t(36)),r[0]))return {};c.pc=0x8002ed88;break;} // 90040024 stw 0, 36(4)
case 0x8002ed88:{if(!word((r[31]+uint32_t(40)),r[3]))return {};c.pc=0x8002ed8c;break;} // 807f0028 lwz 3, 40(31)
case 0x8002ed8c:{if(!word((r[31]+uint32_t(44)),r[0]))return {};c.pc=0x8002ed90;break;} // 801f002c lwz 0, 44(31)
case 0x8002ed90:{if(!store((r[4]+uint32_t(40)),r[3]))return {};c.pc=0x8002ed94;break;} // 90640028 stw 3, 40(4)
case 0x8002ed94:{if(!store((r[4]+uint32_t(44)),r[0]))return {};c.pc=0x8002ed98;break;} // 9004002c stw 0, 44(4)
case 0x8002ed98:{if(!word((r[31]+uint32_t(48)),r[3]))return {};c.pc=0x8002ed9c;break;} // 807f0030 lwz 3, 48(31)
case 0x8002ed9c:{if(!word((r[31]+uint32_t(52)),r[0]))return {};c.pc=0x8002eda0;break;} // 801f0034 lwz 0, 52(31)
case 0x8002eda0:{if(!store((r[4]+uint32_t(48)),r[3]))return {};c.pc=0x8002eda4;break;} // 90640030 stw 3, 48(4)
case 0x8002eda4:{if(!store((r[4]+uint32_t(52)),r[0]))return {};c.pc=0x8002eda8;break;} // 90040034 stw 0, 52(4)
case 0x8002eda8:{if(!word((r[31]+uint32_t(56)),r[0]))return {};c.pc=0x8002edac;break;} // 801f0038 lwz 0, 56(31)
case 0x8002edac:{if(!store((r[4]+uint32_t(56)),r[0]))return {};c.pc=0x8002edb0;break;} // 90040038 stw 0, 56(4)
case 0x8002edb0:{if(!store((r[13]+uint32_t(6292)),r[4]))return {};c.pc=0x8002edb4;break;} // 908d1894 stw 4, 6292(13)
case 0x8002edb4:{c.pc=0;break;} // 480000b0 b 0x8002ee64
