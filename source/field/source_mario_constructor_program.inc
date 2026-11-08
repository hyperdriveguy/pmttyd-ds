// Whole original US revision0 Mario constructor and reached source leaves.
// Generated native continuation graph; verify with compile_mario_constructor.py --check.
// marioInit 8005b5c8/488 SHA256 e3976200ee5fc18a245b234f0b88c4095a0de3108c620034b54df1ea545ee6ce
case 0x8005b5c8:{const auto target=(r[1]+uint32_t(-32));if(!store(target,r[1]))return {};r[1]=target;c.pc=0x8005b5cc;break;} // 9421ffe0
case 0x8005b5cc:{r[0]=c.lr;c.pc=0x8005b5d0;break;} // 7c0802a6
case 0x8005b5d0:{r[4]=uint32_t(-2144600064);c.pc=0x8005b5d4;break;} // 3c80802c
case 0x8005b5d4:{r[3]=uint32_t(0);c.pc=0x8005b5d8;break;} // 38600000
case 0x8005b5d8:{if(!store((r[1]+uint32_t(36)),r[0]))return {};c.pc=0x8005b5dc;break;} // 90010024
case 0x8005b5dc:{if(!store((r[1]+uint32_t(28)),r[31]))return {};c.pc=0x8005b5e0;break;} // 93e1001c
case 0x8005b5e0:{r[31]=r[4]+uint32_t(6304);c.pc=0x8005b5e4;break;} // 3be418a0
case 0x8005b5e4:{r[4]=uint32_t(760);c.pc=0x8005b5e8;break;} // 388002f8
case 0x8005b5e8:{if(!store((r[1]+uint32_t(24)),r[30]))return {};c.pc=0x8005b5ec;break;} // 93c10018
case 0x8005b5ec:{if(!store((r[1]+uint32_t(20)),r[29]))return {};c.pc=0x8005b5f0;break;} // 93a10014
case 0x8005b5f0:{if(!external(0x800300f0,0x8005b5f4))return {};break;} // 4bfd4b01
case 0x8005b5f4:{if(!store((r[13]+uint32_t(6624)),r[3]))return {};c.pc=0x8005b5f8;break;} // 906d19e0
case 0x8005b5f8:{r[4]=uint32_t(0);c.pc=0x8005b5fc;break;} // 38800000
case 0x8005b5fc:{r[5]=uint32_t(760);c.pc=0x8005b600;break;} // 38a002f8
case 0x8005b600:{if(!external(0x800050b4,0x8005b604))return {};break;} // 4bfa9ab5
case 0x8005b604:{if(!word((r[13]+uint32_t(6624)),r[3]))return {};c.pc=0x8005b608;break;} // 806d19e0
case 0x8005b608:{r[0]=uint32_t(-1);c.pc=0x8005b60c;break;} // 3800ffff
case 0x8005b60c:{if(!store((r[3]+uint32_t(636)),r[0]))return {};c.pc=0x8005b610;break;} // 9003027c
case 0x8005b610:{if(!store((r[3]+uint32_t(640)),r[0]))return {};c.pc=0x8005b614;break;} // 90030280
case 0x8005b614:{if(!store((r[3]+uint32_t(644)),r[0]))return {};c.pc=0x8005b618;break;} // 90030284
case 0x8005b618:{if(!store((r[3]+uint32_t(648)),r[0]))return {};c.pc=0x8005b61c;break;} // 90030288
case 0x8005b61c:{if(!store((r[3]+uint32_t(652)),r[0]))return {};c.pc=0x8005b620;break;} // 9003028c
case 0x8005b620:{c.lr=0x8005b624;c.pc=0x800d59c8;break;} // 4807a3a9
case 0x8005b624:{c.lr=0x8005b628;c.pc=0x8009696c;break;} // 4803b349
case 0x8005b628:{c.lr=0x8005b62c;c.pc=0x801a9514;break;} // 4814deed
case 0x8005b62c:{if(!word((r[13]+uint32_t(6624)),r[5]))return {};c.pc=0x8005b630;break;} // 80ad19e0
case 0x8005b630:{r[0]=uint32_t(0);c.pc=0x8005b634;break;} // 38000000
case 0x8005b634:{r[3]=r[13]+uint32_t(-28784);c.pc=0x8005b638;break;} // 386d8f90
case 0x8005b638:{r[4]=uint32_t(2);c.pc=0x8005b63c;break;} // 38800002
case 0x8005b63c:{if(!s.storeByte||!s.storeByte(s.user,(r[5]+uint32_t(61)),uint8_t(r[0])))return {};c.pc=0x8005b640;break;} // 9805003d
case 0x8005b640:{r[5]=uint32_t(0);c.pc=0x8005b644;break;} // 38a00000
case 0x8005b644:{if(!word((r[13]+uint32_t(6624)),r[6]))return {};c.pc=0x8005b648;break;} // 80cd19e0
case 0x8005b648:{if(!byte((r[6]+uint32_t(61)),r[0]))return {};c.pc=0x8005b64c;break;} // 8806003d
case 0x8005b64c:{r[0]=(r[0]&255)<128?(r[0]&255):(r[0]|0xffffff00u);c.pc=0x8005b650;break;} // 7c000774
case 0x8005b650:{r[0]=r[0]*uint32_t(12);c.pc=0x8005b654;break;} // 1c00000c
case 0x8005b654:{if(!word(r[3]+r[0],r[3]))return {};c.pc=0x8005b658;break;} // 7c63002e
case 0x8005b658:{if(!external(0x80039d0c,0x8005b65c))return {};break;} // 4bfde6b5
case 0x8005b65c:{if(!word((r[13]+uint32_t(6624)),r[3]))return {};c.pc=0x8005b660;break;} // 806d19e0
case 0x8005b660:{r[6]=r[13]+uint32_t(-28784);c.pc=0x8005b664;break;} // 38cd8f90
case 0x8005b664:{r[4]=uint32_t(2);c.pc=0x8005b668;break;} // 38800002
case 0x8005b668:{r[5]=uint32_t(0);c.pc=0x8005b66c;break;} // 38a00000
case 0x8005b66c:{if(!byte((r[3]+uint32_t(61)),r[0]))return {};c.pc=0x8005b670;break;} // 8803003d
case 0x8005b670:{r[0]=(r[0]&255)<128?(r[0]&255):(r[0]|0xffffff00u);c.pc=0x8005b674;break;} // 7c000774
case 0x8005b674:{r[0]=r[0]*uint32_t(12);c.pc=0x8005b678;break;} // 1c00000c
case 0x8005b678:{r[3]=r[6]+r[0];c.pc=0x8005b67c;break;} // 7c660214
case 0x8005b67c:{if(!word((r[3]+uint32_t(4)),r[3]))return {};c.pc=0x8005b680;break;} // 80630004
case 0x8005b680:{if(!external(0x80039d0c,0x8005b684))return {};break;} // 4bfde68d
case 0x8005b684:{if(!word((r[13]+uint32_t(6624)),r[3]))return {};c.pc=0x8005b688;break;} // 806d19e0
case 0x8005b688:{r[6]=r[13]+uint32_t(-28784);c.pc=0x8005b68c;break;} // 38cd8f90
case 0x8005b68c:{r[4]=uint32_t(2);c.pc=0x8005b690;break;} // 38800002
case 0x8005b690:{r[5]=uint32_t(0);c.pc=0x8005b694;break;} // 38a00000
case 0x8005b694:{if(!byte((r[3]+uint32_t(61)),r[0]))return {};c.pc=0x8005b698;break;} // 8803003d
case 0x8005b698:{r[0]=(r[0]&255)<128?(r[0]&255):(r[0]|0xffffff00u);c.pc=0x8005b69c;break;} // 7c000774
case 0x8005b69c:{r[0]=r[0]*uint32_t(12);c.pc=0x8005b6a0;break;} // 1c00000c
case 0x8005b6a0:{r[3]=r[6]+r[0];c.pc=0x8005b6a4;break;} // 7c660214
case 0x8005b6a4:{if(!word((r[3]+uint32_t(8)),r[3]))return {};c.pc=0x8005b6a8;break;} // 80630008
case 0x8005b6a8:{if(!external(0x80039d0c,0x8005b6ac))return {};break;} // 4bfde665
case 0x8005b6ac:{if(!word((r[13]+uint32_t(6624)),r[3]))return {};c.pc=0x8005b6b0;break;} // 806d19e0
case 0x8005b6b0:{r[0]=uint32_t(-1);c.pc=0x8005b6b4;break;} // 3800ffff
case 0x8005b6b4:{if(!store((r[3]+uint32_t(556)),r[0]))return {};c.pc=0x8005b6b8;break;} // 9003022c
case 0x8005b6b8:{if(!word((r[13]+uint32_t(6624)),r[3]))return {};c.pc=0x8005b6bc;break;} // 806d19e0
case 0x8005b6bc:{if(!store((r[3]+uint32_t(560)),r[0]))return {};c.pc=0x8005b6c0;break;} // 90030230
case 0x8005b6c0:{if(!word((r[13]+uint32_t(6624)),r[3]))return {};c.pc=0x8005b6c4;break;} // 806d19e0
case 0x8005b6c4:{if(!store((r[3]+uint32_t(564)),r[0]))return {};c.pc=0x8005b6c8;break;} // 90030234
case 0x8005b6c8:{if(!word((r[13]+uint32_t(6624)),r[3]))return {};c.pc=0x8005b6cc;break;} // 806d19e0
case 0x8005b6cc:{if(!store((r[3]+uint32_t(576)),r[0]))return {};c.pc=0x8005b6d0;break;} // 90030240
case 0x8005b6d0:{if(!word((r[13]+uint32_t(6624)),r[3]))return {};c.pc=0x8005b6d4;break;} // 806d19e0
case 0x8005b6d4:{if(!byte((r[3]+uint32_t(60)),r[0]))return {};c.pc=0x8005b6d8;break;} // 8803003c
case 0x8005b6d8:{r[0]=(r[0]&255)<128?(r[0]&255):(r[0]|0xffffff00u);compare(c,0,signedWord(r[0]),0);c.pc=0x8005b6dc;break;} // 7c000775
case 0x8005b6dc:{c.pc=(true)&&((c.condition[2]==false))?0x8005b6f8:0x8005b6e0;break;} // 4082001c
case 0x8005b6e0:{if(!word((r[2]+uint32_t(-29752)),f[1]))return {};c.pc=0x8005b6e4;break;} // c0228bc8
case 0x8005b6e4:{if(!word((r[2]+uint32_t(-29576)),f[0]))return {};c.pc=0x8005b6e8;break;} // c0028c78
case 0x8005b6e8:{if(!store((r[3]+uint32_t(388)),f[1]))return {};c.pc=0x8005b6ec;break;} // d0230184
case 0x8005b6ec:{if(!word((r[13]+uint32_t(6624)),r[3]))return {};c.pc=0x8005b6f0;break;} // 806d19e0
case 0x8005b6f0:{if(!store((r[3]+uint32_t(392)),f[0]))return {};c.pc=0x8005b6f4;break;} // d0030188
case 0x8005b6f4:{c.pc=0x8005b70c;break;} // 48000018
case 0x8005b6f8:{if(!word((r[2]+uint32_t(-29752)),f[1]))return {};c.pc=0x8005b6fc;break;} // c0228bc8
case 0x8005b6fc:{if(!word((r[2]+uint32_t(-29756)),f[0]))return {};c.pc=0x8005b700;break;} // c0028bc4
case 0x8005b700:{if(!store((r[3]+uint32_t(388)),f[1]))return {};c.pc=0x8005b704;break;} // d0230184
case 0x8005b704:{if(!word((r[13]+uint32_t(6624)),r[3]))return {};c.pc=0x8005b708;break;} // 806d19e0
case 0x8005b708:{if(!store((r[3]+uint32_t(392)),f[0]))return {};c.pc=0x8005b70c;break;} // d0030188
case 0x8005b70c:{if(!word((r[13]+uint32_t(6624)),r[3]))return {};c.pc=0x8005b710;break;} // 806d19e0
case 0x8005b710:{r[0]=uint32_t(-1);c.pc=0x8005b714;break;} // 3800ffff
case 0x8005b714:{if(!word((r[2]+uint32_t(-29624)),f[1]))return {};c.pc=0x8005b718;break;} // c0228c48
case 0x8005b718:{if(!s.storeByte||!s.storeByte(s.user,(r[3]+uint32_t(581)),uint8_t(r[0])))return {};c.pc=0x8005b71c;break;} // 98030245
case 0x8005b71c:{if(!word((r[2]+uint32_t(-29568)),f[0]))return {};c.pc=0x8005b720;break;} // c0028c80
case 0x8005b720:{if(!word((r[13]+uint32_t(6624)),r[3]))return {};c.pc=0x8005b724;break;} // 806d19e0
case 0x8005b724:{if(!s.storeByte||!s.storeByte(s.user,(r[3]+uint32_t(582)),uint8_t(r[0])))return {};c.pc=0x8005b728;break;} // 98030246
case 0x8005b728:{if(!word((r[13]+uint32_t(6624)),r[3]))return {};c.pc=0x8005b72c;break;} // 806d19e0
case 0x8005b72c:{if(!store((r[3]+uint32_t(440)),f[1]))return {};c.pc=0x8005b730;break;} // d02301b8
case 0x8005b730:{if(!word((r[13]+uint32_t(6624)),r[3]))return {};c.pc=0x8005b734;break;} // 806d19e0
case 0x8005b734:{if(!store((r[3]+uint32_t(444)),f[0]))return {};c.pc=0x8005b738;break;} // d00301bc
case 0x8005b738:{if(!word((r[13]+uint32_t(6624)),r[4]))return {};c.pc=0x8005b73c;break;} // 808d19e0
case 0x8005b73c:{if(!word((r[31]+uint32_t(184)),r[3]))return {};c.pc=0x8005b740;break;} // 807f00b8
case 0x8005b740:{if(!word((r[31]+uint32_t(188)),r[0]))return {};c.pc=0x8005b744;break;} // 801f00bc
case 0x8005b744:{if(!store((r[4]+uint32_t(200)),r[3]))return {};c.pc=0x8005b748;break;} // 906400c8
case 0x8005b748:{if(!store((r[4]+uint32_t(204)),r[0]))return {};c.pc=0x8005b74c;break;} // 900400cc
case 0x8005b74c:{if(!word((r[31]+uint32_t(192)),r[0]))return {};c.pc=0x8005b750;break;} // 801f00c0
case 0x8005b750:{if(!store((r[4]+uint32_t(208)),r[0]))return {};c.pc=0x8005b754;break;} // 900400d0
case 0x8005b754:{if(!word((r[13]+uint32_t(6624)),r[4]))return {};c.pc=0x8005b758;break;} // 808d19e0
case 0x8005b758:{if(!word((r[4]+uint32_t(140)),r[3]))return {};c.pc=0x8005b75c;break;} // 8064008c
case 0x8005b75c:{if(!word((r[4]+uint32_t(144)),r[0]))return {};c.pc=0x8005b760;break;} // 80040090
case 0x8005b760:{if(!store((r[4]+uint32_t(272)),r[3]))return {};c.pc=0x8005b764;break;} // 90640110
case 0x8005b764:{if(!store((r[4]+uint32_t(276)),r[0]))return {};c.pc=0x8005b768;break;} // 90040114
case 0x8005b768:{if(!word((r[4]+uint32_t(148)),r[0]))return {};c.pc=0x8005b76c;break;} // 80040094
case 0x8005b76c:{if(!store((r[4]+uint32_t(280)),r[0]))return {};c.pc=0x8005b770;break;} // 90040118
case 0x8005b770:{if(!word((r[13]+uint32_t(6624)),r[3]))return {};c.pc=0x8005b774;break;} // 806d19e0
case 0x8005b774:{if(!store((r[3]+uint32_t(448)),f[1]))return {};c.pc=0x8005b778;break;} // d02301c0
case 0x8005b778:{if(!word((r[13]+uint32_t(6624)),r[30]))return {};c.pc=0x8005b77c;break;} // 83cd19e0
case 0x8005b77c:{if(!word((r[30]+uint32_t(4)),r[0]))return {};c.pc=0x8005b780;break;} // 801e0004
case 0x8005b780:{r[0]=rotate(r[0],0)&0x00000002;compare(c,0,signedWord(r[0]),0);c.pc=0x8005b784;break;} // 540007bd
case 0x8005b784:{c.pc=(true)&&((c.condition[2]==false))?0x8005b954:0x8005b788;break;} // 408201d0
case 0x8005b788:{if(!word((r[30]+uint32_t(24)),r[3]))return {};c.pc=0x8005b78c;break;} // 807e0018
case 0x8005b78c:{compare(c,0,uint64_t(r[3]),0);c.pc=0x8005b790;break;} // 28030000
case 0x8005b790:{c.pc=(true)&&((c.condition[2]==true))?0x8005b7a4:0x8005b794;break;} // 41820014
case 0x8005b794:{r[4]=r[2]+uint32_t(-29864);c.pc=0x8005b798;break;} // 38828b58
case 0x8005b798:{if(!external(0x8026d464,0x8005b79c))return {};break;} // 48211ccd
case 0x8005b79c:{compare(c,0,signedWord(r[3]),0);c.pc=0x8005b7a0;break;} // 2c030000
case 0x8005b7a0:{c.pc=(true)&&((c.condition[2]==true))?0x8005b954:0x8005b7a4;break;} // 418201b4
case 0x8005b7a4:{if(!word((r[30]+uint32_t(0)),r[0]))return {};c.pc=0x8005b7a8;break;} // 801e0000
case 0x8005b7a8:{r[0]=rotate(r[0],0)&0x80000000;compare(c,0,signedWord(r[0]),0);c.pc=0x8005b7ac;break;} // 54000001
case 0x8005b7ac:{c.pc=(true)&&((c.condition[2]==false))?0x8005b7dc:0x8005b7b0;break;} // 40820030
case 0x8005b7b0:{r[3]=r[2]+uint32_t(-29864);c.pc=0x8005b7b4;break;} // 38628b58
case 0x8005b7b4:{r[0]=uint32_t(0);c.pc=0x8005b7b8;break;} // 38000000
case 0x8005b7b8:{if(!store((r[30]+uint32_t(24)),r[3]))return {};c.pc=0x8005b7bc;break;} // 907e0018
case 0x8005b7bc:{if(!s.storeHalf||!s.storeHalf(s.user,(r[30]+uint32_t(40)),uint16_t(r[0])))return {};c.pc=0x8005b7c0;break;} // b01e0028
case 0x8005b7c0:{if(!word((r[30]+uint32_t(4)),r[0]))return {};c.pc=0x8005b7c4;break;} // 801e0004
case 0x8005b7c4:{r[0]=rotate(r[0],0)&0xcfffffff;c.pc=0x8005b7c8;break;} // 54000102
case 0x8005b7c8:{if(!store((r[30]+uint32_t(4)),r[0]))return {};c.pc=0x8005b7cc;break;} // 901e0004
case 0x8005b7cc:{if(!word((r[30]+uint32_t(12)),r[0]))return {};c.pc=0x8005b7d0;break;} // 801e000c
case 0x8005b7d0:{r[0]=r[0]|0x00001000;c.pc=0x8005b7d4;break;} // 60001000
case 0x8005b7d4:{if(!store((r[30]+uint32_t(12)),r[0]))return {};c.pc=0x8005b7d8;break;} // 901e000c
case 0x8005b7d8:{c.pc=0x8005b954;break;} // 4800017c
case 0x8005b7dc:{r[29]=uint32_t(0);c.pc=0x8005b7e0;break;} // 3ba00000
case 0x8005b7e0:{r[3]=r[2]+uint32_t(-29872);c.pc=0x8005b7e4;break;} // 38628b50
case 0x8005b7e4:{r[4]=r[2]+uint32_t(-29864);c.pc=0x8005b7e8;break;} // 38828b58
case 0x8005b7e8:{if(!external(0x8026d464,0x8005b7ec))return {};break;} // 48211c7d
case 0x8005b7ec:{compare(c,0,signedWord(r[3]),0);c.pc=0x8005b7f0;break;} // 2c030000
case 0x8005b7f0:{c.pc=(true)&&((c.condition[2]==false))?0x8005b814:0x8005b7f4;break;} // 40820024
case 0x8005b7f4:{if(!word((r[13]+uint32_t(6624)),r[4]))return {};c.pc=0x8005b7f8;break;} // 808d19e0
case 0x8005b7f8:{r[3]=uint32_t(-2144272384);c.pc=0x8005b7fc;break;} // 3c608031
case 0x8005b7fc:{r[3]=r[3]+uint32_t(-26808);c.pc=0x8005b800;break;} // 38639748
case 0x8005b800:{if(!byte((r[4]+uint32_t(61)),r[0]))return {};c.pc=0x8005b804;break;} // 8804003d
case 0x8005b804:{r[0]=(r[0]&255)<128?(r[0]&255):(r[0]|0xffffff00u);c.pc=0x8005b808;break;} // 7c000774
case 0x8005b808:{r[0]=r[0]*uint32_t(24);c.pc=0x8005b80c;break;} // 1c000018
case 0x8005b80c:{if(!word(r[3]+r[0],r[29]))return {};c.pc=0x8005b810;break;} // 7fa3002e
case 0x8005b810:{c.pc=0x8005b928;break;} // 48000118
case 0x8005b814:{r[3]=r[2]+uint32_t(-29864);c.pc=0x8005b818;break;} // 38628b58
case 0x8005b818:{r[4]=r[2]+uint32_t(-29864);c.pc=0x8005b81c;break;} // 38828b58
case 0x8005b81c:{if(!external(0x8026d464,0x8005b820))return {};break;} // 48211c49
case 0x8005b820:{compare(c,0,signedWord(r[3]),0);c.pc=0x8005b824;break;} // 2c030000
case 0x8005b824:{c.pc=(true)&&((c.condition[2]==false))?0x8005b84c:0x8005b828;break;} // 40820028
case 0x8005b828:{if(!word((r[13]+uint32_t(6624)),r[3]))return {};c.pc=0x8005b82c;break;} // 806d19e0
case 0x8005b82c:{r[4]=uint32_t(-2144272384);c.pc=0x8005b830;break;} // 3c808031
case 0x8005b830:{r[4]=r[4]+uint32_t(-26808);c.pc=0x8005b834;break;} // 38849748
case 0x8005b834:{if(!byte((r[3]+uint32_t(61)),r[0]))return {};c.pc=0x8005b838;break;} // 8803003d
case 0x8005b838:{r[0]=(r[0]&255)<128?(r[0]&255):(r[0]|0xffffff00u);c.pc=0x8005b83c;break;} // 7c000774
case 0x8005b83c:{r[0]=r[0]*uint32_t(24);c.pc=0x8005b840;break;} // 1c000018
case 0x8005b840:{r[3]=r[4]+r[0];c.pc=0x8005b844;break;} // 7c640214
case 0x8005b844:{if(!word((r[3]+uint32_t(4)),r[29]))return {};c.pc=0x8005b848;break;} // 83a30004
case 0x8005b848:{c.pc=0x8005b928;break;} // 480000e0
case 0x8005b84c:{r[3]=r[2]+uint32_t(-29856);c.pc=0x8005b850;break;} // 38628b60
case 0x8005b850:{r[4]=r[2]+uint32_t(-29864);c.pc=0x8005b854;break;} // 38828b58
case 0x8005b854:{if(!external(0x8026d464,0x8005b858))return {};break;} // 48211c11
case 0x8005b858:{compare(c,0,signedWord(r[3]),0);c.pc=0x8005b85c;break;} // 2c030000
case 0x8005b85c:{c.pc=(true)&&((c.condition[2]==false))?0x8005b884:0x8005b860;break;} // 40820028
case 0x8005b860:{if(!word((r[13]+uint32_t(6624)),r[3]))return {};c.pc=0x8005b864;break;} // 806d19e0
case 0x8005b864:{r[4]=uint32_t(-2144272384);c.pc=0x8005b868;break;} // 3c808031
case 0x8005b868:{r[4]=r[4]+uint32_t(-26808);c.pc=0x8005b86c;break;} // 38849748
case 0x8005b86c:{if(!byte((r[3]+uint32_t(61)),r[0]))return {};c.pc=0x8005b870;break;} // 8803003d
case 0x8005b870:{r[0]=(r[0]&255)<128?(r[0]&255):(r[0]|0xffffff00u);c.pc=0x8005b874;break;} // 7c000774
case 0x8005b874:{r[0]=r[0]*uint32_t(24);c.pc=0x8005b878;break;} // 1c000018
case 0x8005b878:{r[3]=r[4]+r[0];c.pc=0x8005b87c;break;} // 7c640214
case 0x8005b87c:{if(!word((r[3]+uint32_t(8)),r[29]))return {};c.pc=0x8005b880;break;} // 83a30008
case 0x8005b880:{c.pc=0x8005b928;break;} // 480000a8
case 0x8005b884:{r[3]=r[2]+uint32_t(-29848);c.pc=0x8005b888;break;} // 38628b68
case 0x8005b888:{r[4]=r[2]+uint32_t(-29864);c.pc=0x8005b88c;break;} // 38828b58
case 0x8005b88c:{if(!external(0x8026d464,0x8005b890))return {};break;} // 48211bd9
case 0x8005b890:{compare(c,0,signedWord(r[3]),0);c.pc=0x8005b894;break;} // 2c030000
case 0x8005b894:{c.pc=(true)&&((c.condition[2]==false))?0x8005b8bc:0x8005b898;break;} // 40820028
case 0x8005b898:{if(!word((r[13]+uint32_t(6624)),r[3]))return {};c.pc=0x8005b89c;break;} // 806d19e0
case 0x8005b89c:{r[4]=uint32_t(-2144272384);c.pc=0x8005b8a0;break;} // 3c808031
case 0x8005b8a0:{r[4]=r[4]+uint32_t(-26808);c.pc=0x8005b8a4;break;} // 38849748
case 0x8005b8a4:{if(!byte((r[3]+uint32_t(61)),r[0]))return {};c.pc=0x8005b8a8;break;} // 8803003d
case 0x8005b8a8:{r[0]=(r[0]&255)<128?(r[0]&255):(r[0]|0xffffff00u);c.pc=0x8005b8ac;break;} // 7c000774
case 0x8005b8ac:{r[0]=r[0]*uint32_t(24);c.pc=0x8005b8b0;break;} // 1c000018
case 0x8005b8b0:{r[3]=r[4]+r[0];c.pc=0x8005b8b4;break;} // 7c640214
case 0x8005b8b4:{if(!word((r[3]+uint32_t(12)),r[29]))return {};c.pc=0x8005b8b8;break;} // 83a3000c
case 0x8005b8b8:{c.pc=0x8005b928;break;} // 48000070
case 0x8005b8bc:{r[3]=r[31]+uint32_t(644);c.pc=0x8005b8c0;break;} // 387f0284
case 0x8005b8c0:{r[4]=r[2]+uint32_t(-29864);c.pc=0x8005b8c4;break;} // 38828b58
case 0x8005b8c4:{if(!external(0x8026d464,0x8005b8c8))return {};break;} // 48211ba1
case 0x8005b8c8:{compare(c,0,signedWord(r[3]),0);c.pc=0x8005b8cc;break;} // 2c030000
case 0x8005b8cc:{c.pc=(true)&&((c.condition[2]==false))?0x8005b8f4:0x8005b8d0;break;} // 40820028
case 0x8005b8d0:{if(!word((r[13]+uint32_t(6624)),r[3]))return {};c.pc=0x8005b8d4;break;} // 806d19e0
case 0x8005b8d4:{r[4]=uint32_t(-2144272384);c.pc=0x8005b8d8;break;} // 3c808031
case 0x8005b8d8:{r[4]=r[4]+uint32_t(-26808);c.pc=0x8005b8dc;break;} // 38849748
case 0x8005b8dc:{if(!byte((r[3]+uint32_t(61)),r[0]))return {};c.pc=0x8005b8e0;break;} // 8803003d
case 0x8005b8e0:{r[0]=(r[0]&255)<128?(r[0]&255):(r[0]|0xffffff00u);c.pc=0x8005b8e4;break;} // 7c000774
case 0x8005b8e4:{r[0]=r[0]*uint32_t(24);c.pc=0x8005b8e8;break;} // 1c000018
case 0x8005b8e8:{r[3]=r[4]+r[0];c.pc=0x8005b8ec;break;} // 7c640214
case 0x8005b8ec:{if(!word((r[3]+uint32_t(20)),r[29]))return {};c.pc=0x8005b8f0;break;} // 83a30014
case 0x8005b8f0:{c.pc=0x8005b928;break;} // 48000038
case 0x8005b8f4:{r[3]=r[31]+uint32_t(660);c.pc=0x8005b8f8;break;} // 387f0294
case 0x8005b8f8:{r[4]=r[2]+uint32_t(-29864);c.pc=0x8005b8fc;break;} // 38828b58
case 0x8005b8fc:{if(!external(0x8026d464,0x8005b900))return {};break;} // 48211b69
case 0x8005b900:{compare(c,0,signedWord(r[3]),0);c.pc=0x8005b904;break;} // 2c030000
case 0x8005b904:{c.pc=(true)&&((c.condition[2]==false))?0x8005b928:0x8005b908;break;} // 40820024
case 0x8005b908:{if(!word((r[13]+uint32_t(6624)),r[3]))return {};c.pc=0x8005b90c;break;} // 806d19e0
case 0x8005b90c:{r[4]=uint32_t(-2144272384);c.pc=0x8005b910;break;} // 3c808031
case 0x8005b910:{r[4]=r[4]+uint32_t(-26808);c.pc=0x8005b914;break;} // 38849748
case 0x8005b914:{if(!byte((r[3]+uint32_t(61)),r[0]))return {};c.pc=0x8005b918;break;} // 8803003d
case 0x8005b918:{r[0]=(r[0]&255)<128?(r[0]&255):(r[0]|0xffffff00u);c.pc=0x8005b91c;break;} // 7c000774
case 0x8005b91c:{r[0]=r[0]*uint32_t(24);c.pc=0x8005b920;break;} // 1c000018
case 0x8005b920:{r[3]=r[4]+r[0];c.pc=0x8005b924;break;} // 7c640214
case 0x8005b924:{if(!word((r[3]+uint32_t(20)),r[29]))return {};c.pc=0x8005b928;break;} // 83a30014
case 0x8005b928:{compare(c,0,uint64_t(r[29]),0);c.pc=0x8005b92c;break;} // 281d0000
case 0x8005b92c:{c.pc=(true)&&((c.condition[2]==true))?0x8005b954:0x8005b930;break;} // 41820028
case 0x8005b930:{if(!store((r[30]+uint32_t(24)),r[29]))return {};c.pc=0x8005b934;break;} // 93be0018
case 0x8005b934:{r[0]=uint32_t(0);c.pc=0x8005b938;break;} // 38000000
case 0x8005b938:{if(!s.storeHalf||!s.storeHalf(s.user,(r[30]+uint32_t(40)),uint16_t(r[0])))return {};c.pc=0x8005b93c;break;} // b01e0028
case 0x8005b93c:{if(!word((r[30]+uint32_t(4)),r[0]))return {};c.pc=0x8005b940;break;} // 801e0004
case 0x8005b940:{r[0]=rotate(r[0],0)&0xcfffffff;c.pc=0x8005b944;break;} // 54000102
case 0x8005b944:{if(!store((r[30]+uint32_t(4)),r[0]))return {};c.pc=0x8005b948;break;} // 901e0004
case 0x8005b948:{if(!word((r[30]+uint32_t(12)),r[0]))return {};c.pc=0x8005b94c;break;} // 801e000c
case 0x8005b94c:{r[0]=r[0]|0x00001000;c.pc=0x8005b950;break;} // 60001000
case 0x8005b950:{if(!store((r[30]+uint32_t(12)),r[0]))return {};c.pc=0x8005b954;break;} // 901e000c
case 0x8005b954:{if(!word((r[13]+uint32_t(6624)),r[4]))return {};c.pc=0x8005b958;break;} // 808d19e0
case 0x8005b958:{r[0]=uint32_t(0);c.pc=0x8005b95c;break;} // 38000000
case 0x8005b95c:{r[3]=uint32_t(0);c.pc=0x8005b960;break;} // 38600000
case 0x8005b960:{if(!store((r[4]+uint32_t(32)),r[0]))return {};c.pc=0x8005b964;break;} // 90040020
case 0x8005b964:{if(!word((r[13]+uint32_t(6624)),r[5]))return {};c.pc=0x8005b968;break;} // 80ad19e0
case 0x8005b968:{if(!word((r[31]+uint32_t(196)),r[4]))return {};c.pc=0x8005b96c;break;} // 809f00c4
case 0x8005b96c:{if(!word((r[31]+uint32_t(200)),r[0]))return {};c.pc=0x8005b970;break;} // 801f00c8
case 0x8005b970:{if(!store((r[5]+uint32_t(740)),r[4]))return {};c.pc=0x8005b974;break;} // 908502e4
case 0x8005b974:{if(!store((r[5]+uint32_t(744)),r[0]))return {};c.pc=0x8005b978;break;} // 900502e8
case 0x8005b978:{if(!word((r[31]+uint32_t(204)),r[0]))return {};c.pc=0x8005b97c;break;} // 801f00cc
case 0x8005b97c:{if(!store((r[5]+uint32_t(748)),r[0]))return {};c.pc=0x8005b980;break;} // 900502ec
case 0x8005b980:{c.lr=0x8005b984;c.pc=0x8009455c;break;} // 48038bdd
case 0x8005b984:{c.lr=0x8005b988;c.pc=0x8012f550;break;} // 480d3bcd
case 0x8005b988:{c.lr=0x8005b98c;c.pc=0x80095a4c;break;} // 4803a0c5
case 0x8005b98c:{r[0]=uint32_t(0);c.pc=0x8005b990;break;} // 38000000
case 0x8005b990:{r[3]=uint32_t(-2143420416);c.pc=0x8005b994;break;} // 3c60803e
case 0x8005b994:{if(!store((r[13]+uint32_t(6608)),r[0]))return {};c.pc=0x8005b998;break;} // 900d19d0
case 0x8005b998:{r[5]=r[3]+uint32_t(-26552);c.pc=0x8005b99c;break;} // 38a39848
case 0x8005b99c:{if(!word((r[13]+uint32_t(6624)),r[6]))return {};c.pc=0x8005b9a0;break;} // 80cd19e0
case 0x8005b9a0:{if(!word((r[6]+uint32_t(140)),r[4]))return {};c.pc=0x8005b9a4;break;} // 8086008c
case 0x8005b9a4:{if(!word((r[6]+uint32_t(144)),r[3]))return {};c.pc=0x8005b9a8;break;} // 80660090
case 0x8005b9a8:{if(!store((r[5]+uint32_t(0)),r[4]))return {};c.pc=0x8005b9ac;break;} // 90850000
case 0x8005b9ac:{if(!store((r[5]+uint32_t(4)),r[3]))return {};c.pc=0x8005b9b0;break;} // 90650004
case 0x8005b9b0:{if(!word((r[6]+uint32_t(148)),r[0]))return {};c.pc=0x8005b9b4;break;} // 80060094
case 0x8005b9b4:{if(!store((r[5]+uint32_t(8)),r[0]))return {};c.pc=0x8005b9b8;break;} // 90050008
case 0x8005b9b8:{if(!store((r[5]+uint32_t(12)),r[4]))return {};c.pc=0x8005b9bc;break;} // 9085000c
case 0x8005b9bc:{if(!store((r[5]+uint32_t(16)),r[3]))return {};c.pc=0x8005b9c0;break;} // 90650010
case 0x8005b9c0:{if(!store((r[5]+uint32_t(20)),r[0]))return {};c.pc=0x8005b9c4;break;} // 90050014
case 0x8005b9c4:{if(!store((r[5]+uint32_t(24)),r[4]))return {};c.pc=0x8005b9c8;break;} // 90850018
case 0x8005b9c8:{if(!store((r[5]+uint32_t(28)),r[3]))return {};c.pc=0x8005b9cc;break;} // 9065001c
case 0x8005b9cc:{if(!store((r[5]+uint32_t(32)),r[0]))return {};c.pc=0x8005b9d0;break;} // 90050020
case 0x8005b9d0:{if(!store((r[5]+uint32_t(36)),r[4]))return {};c.pc=0x8005b9d4;break;} // 90850024
case 0x8005b9d4:{if(!store((r[5]+uint32_t(40)),r[3]))return {};c.pc=0x8005b9d8;break;} // 90650028
case 0x8005b9d8:{if(!store((r[5]+uint32_t(44)),r[0]))return {};c.pc=0x8005b9dc;break;} // 9005002c
case 0x8005b9dc:{if(!store((r[5]+uint32_t(48)),r[4]))return {};c.pc=0x8005b9e0;break;} // 90850030
case 0x8005b9e0:{if(!store((r[5]+uint32_t(52)),r[3]))return {};c.pc=0x8005b9e4;break;} // 90650034
case 0x8005b9e4:{if(!store((r[5]+uint32_t(56)),r[0]))return {};c.pc=0x8005b9e8;break;} // 90050038
case 0x8005b9e8:{if(!store((r[5]+uint32_t(60)),r[4]))return {};c.pc=0x8005b9ec;break;} // 9085003c
case 0x8005b9ec:{if(!store((r[5]+uint32_t(64)),r[3]))return {};c.pc=0x8005b9f0;break;} // 90650040
case 0x8005b9f0:{if(!store((r[5]+uint32_t(68)),r[0]))return {};c.pc=0x8005b9f4;break;} // 90050044
case 0x8005b9f4:{if(!store((r[5]+uint32_t(72)),r[4]))return {};c.pc=0x8005b9f8;break;} // 90850048
case 0x8005b9f8:{if(!store((r[5]+uint32_t(76)),r[3]))return {};c.pc=0x8005b9fc;break;} // 9065004c
case 0x8005b9fc:{if(!store((r[5]+uint32_t(80)),r[0]))return {};c.pc=0x8005ba00;break;} // 90050050
case 0x8005ba00:{if(!store((r[5]+uint32_t(84)),r[4]))return {};c.pc=0x8005ba04;break;} // 90850054
case 0x8005ba04:{if(!store((r[5]+uint32_t(88)),r[3]))return {};c.pc=0x8005ba08;break;} // 90650058
case 0x8005ba08:{if(!store((r[5]+uint32_t(92)),r[0]))return {};c.pc=0x8005ba0c;break;} // 9005005c
case 0x8005ba0c:{c.lr=0x8005ba10;c.pc=0x801a95a8;break;} // 4814db9d
case 0x8005ba10:{c.lr=0x8005ba14;c.pc=0x801a9544;break;} // 4814db35
case 0x8005ba14:{c.lr=0x8005ba18;c.pc=0x801a9488;break;} // 4814da75
case 0x8005ba18:{r[3]=uint32_t(9);c.pc=0x8005ba1c;break;} // 38600009
case 0x8005ba1c:{c.lr=0x8005ba20;c.pc=0x800d4ed4;break;} // 480794b9
case 0x8005ba20:{compare(c,0,signedWord(r[3]),0);c.pc=0x8005ba24;break;} // 2c030000
case 0x8005ba24:{c.pc=(true)&&((c.condition[2]==false))?0x8005ba30:0x8005ba28;break;} // 4082000c
case 0x8005ba28:{r[3]=uint32_t(9);c.pc=0x8005ba2c;break;} // 38600009
case 0x8005ba2c:{c.lr=0x8005ba30;c.pc=0x800d50cc;break;} // 480796a1
case 0x8005ba30:{c.lr=0x8005ba34;c.pc=0x800bd6ac;break;} // 48061c7d
case 0x8005ba34:{if(!word((r[1]+uint32_t(36)),r[0]))return {};c.pc=0x8005ba38;break;} // 80010024
case 0x8005ba38:{if(!word((r[1]+uint32_t(28)),r[31]))return {};c.pc=0x8005ba3c;break;} // 83e1001c
case 0x8005ba3c:{if(!word((r[1]+uint32_t(24)),r[30]))return {};c.pc=0x8005ba40;break;} // 83c10018
case 0x8005ba40:{if(!word((r[1]+uint32_t(20)),r[29]))return {};c.pc=0x8005ba44;break;} // 83a10014
case 0x8005ba44:{c.lr=r[0];c.pc=0x8005ba48;break;} // 7c0803a6
case 0x8005ba48:{r[1]=r[1]+uint32_t(32);c.pc=0x8005ba4c;break;} // 38210020
case 0x8005ba4c:{c.pc=c.lr;break;} // 4e800020
// marioGetRubInit 80095a4c/40 SHA256 1126f0b69a40a2600118bbb8f65412efad5732656dcbb6177fcd40568de38bfa
case 0x80095a4c:{const auto target=(r[1]+uint32_t(-16));if(!store(target,r[1]))return {};r[1]=target;c.pc=0x80095a50;break;} // 9421fff0
case 0x80095a50:{r[0]=c.lr;c.pc=0x80095a54;break;} // 7c0802a6
case 0x80095a54:{r[3]=uint32_t(-2143420416);c.pc=0x80095a58;break;} // 3c60803e
case 0x80095a58:{r[4]=uint32_t(0);c.pc=0x80095a5c;break;} // 38800000
case 0x80095a5c:{if(!store((r[1]+uint32_t(20)),r[0]))return {};c.pc=0x80095a60;break;} // 90010014
case 0x80095a60:{r[3]=r[3]+uint32_t(-15832);c.pc=0x80095a64;break;} // 3863c228
case 0x80095a64:{r[5]=uint32_t(104);c.pc=0x80095a68;break;} // 38a00068
case 0x80095a68:{if(!external(0x800050b4,0x80095a6c))return {};break;} // 4bf6f64d
case 0x80095a6c:{r[3]=uint32_t(-2143420416);c.pc=0x80095a70;break;} // 3c60803e
case 0x80095a70:{r[0]=uint32_t(-1);c.pc=0x80095a74;break;} // 3800ffff
case 0x80095a74:{r[3]=r[3]+uint32_t(-15832);c.pc=0x80095a78;break;} // 3863c228
case 0x80095a78:{if(!store((r[3]+uint32_t(32)),r[0]))return {};c.pc=0x80095a7c;break;} // 90030020
case 0x80095a7c:{if(!word((r[1]+uint32_t(20)),r[0]))return {};c.pc=0x80095a80;break;} // 80010014
case 0x80095a80:{c.lr=r[0];c.pc=0x80095a84;break;} // 7c0803a6
case 0x80095a84:{r[1]=r[1]+uint32_t(16);c.pc=0x80095a88;break;} // 38210010
case 0x80095a88:{c.pc=c.lr;break;} // 4e800020
// marioBaloonInit 8012f550/30 SHA256 7d2da3c114b275afc2fa1c7622802ee02a3edf9f545c6892fd7ca9ab3ddc008c
case 0x8012f550:{const auto target=(r[1]+uint32_t(-16));if(!store(target,r[1]))return {};r[1]=target;c.pc=0x8012f554;break;} // 9421fff0
case 0x8012f554:{r[0]=c.lr;c.pc=0x8012f558;break;} // 7c0802a6
case 0x8012f558:{if(!store((r[1]+uint32_t(20)),r[0]))return {};c.pc=0x8012f55c;break;} // 90010014
case 0x8012f55c:{if(!store((r[1]+uint32_t(12)),r[31]))return {};c.pc=0x8012f560;break;} // 93e1000c
case 0x8012f560:{r[31]=uint32_t(0);c.pc=0x8012f564;break;} // 3be00000
case 0x8012f564:{c.lr=0x8012f568;c.pc=0x8005e0c0;break;} // 4bf2eb5d
case 0x8012f568:{if(!store((r[3]+uint32_t(612)),r[31]))return {};c.pc=0x8012f56c;break;} // 93e30264
case 0x8012f56c:{if(!word((r[1]+uint32_t(20)),r[0]))return {};c.pc=0x8012f570;break;} // 80010014
case 0x8012f570:{if(!word((r[1]+uint32_t(12)),r[31]))return {};c.pc=0x8012f574;break;} // 83e1000c
case 0x8012f574:{c.lr=r[0];c.pc=0x8012f578;break;} // 7c0803a6
case 0x8012f578:{r[1]=r[1]+uint32_t(16);c.pc=0x8012f57c;break;} // 38210010
case 0x8012f57c:{c.pc=c.lr;break;} // 4e800020
// marioResetGravity 8009696c/38 SHA256 8b620c3bf865c904e3d25c626609a6766101b83d4bdaccdbccde1e61588b6ebb
case 0x8009696c:{const auto target=(r[1]+uint32_t(-32));if(!store(target,r[1]))return {};r[1]=target;c.pc=0x80096970;break;} // 9421ffe0
case 0x80096970:{r[0]=c.lr;c.pc=0x80096974;break;} // 7c0802a6
case 0x80096974:{if(!store((r[1]+uint32_t(36)),r[0]))return {};c.pc=0x80096978;break;} // 90010024
case 0x80096978:{const auto value=doubleBits(double(scalar(f[31])));while(c.step<2){if(!store((r[1]+uint32_t(16))+4*c.step,uint32_t(value>>(c.step?0:32))))return {};++c.step;}c.step=0;c.pc=0x8009697c;break;} // dbe10010
case 0x8009697c:{while(c.step<2){if(!store((r[1]+uint32_t(24))+4*c.step,c.step?c.paired[31]:f[31]))return {};++c.step;}c.step=0;c.pc=0x80096980;break;} // f3e10018
case 0x80096980:{if(!word((r[2]+uint32_t(-27084)),f[31]))return {};c.pc=0x80096984;break;} // c3e29634
case 0x80096984:{c.lr=0x80096988;c.pc=0x8005e0c0;break;} // 4bfc773d
case 0x80096988:{if(!store((r[3]+uint32_t(552)),f[31]))return {};c.pc=0x8009698c;break;} // d3e30228
case 0x8009698c:{if(!c.step){if(!word((r[1]+uint32_t(24)),f[31]))return {};c.step=1;}if(!word((r[1]+uint32_t(24))+4,c.paired[31]))return {};c.step=0;c.pc=0x80096990;break;} // e3e10018
case 0x80096990:{if(!word((r[1]+uint32_t(36)),r[0]))return {};c.pc=0x80096994;break;} // 80010024
case 0x80096994:{if(!c.step){if(!word((r[1]+uint32_t(16)),c.transfer))return {};c.step=1;}uint32_t low;if(!word((r[1]+uint32_t(16))+4,low))return {};f[31]=bits(float(doubleScalar(uint64_t(c.transfer)<<32|low)));c.step=0;c.pc=0x80096998;break;} // cbe10010
case 0x80096998:{c.lr=r[0];c.pc=0x8009699c;break;} // 7c0803a6
case 0x8009699c:{r[1]=r[1]+uint32_t(32);c.pc=0x800969a0;break;} // 38210020
case 0x800969a0:{c.pc=c.lr;break;} // 4e800020
// marioChgMot 8009455c/c0 SHA256 cb0d860ff6a94e5e6d0da5dded999fef1e6b881c2169730ce9d09c7f0439da26
case 0x8009455c:{const auto target=(r[1]+uint32_t(-16));if(!store(target,r[1]))return {};r[1]=target;c.pc=0x80094560;break;} // 9421fff0
case 0x80094560:{r[0]=c.lr;c.pc=0x80094564;break;} // 7c0802a6
case 0x80094564:{if(!store((r[1]+uint32_t(20)),r[0]))return {};c.pc=0x80094568;break;} // 90010014
case 0x80094568:{if(!store((r[1]+uint32_t(12)),r[31]))return {};c.pc=0x8009456c;break;} // 93e1000c
case 0x8009456c:{if(!store((r[1]+uint32_t(8)),r[30]))return {};c.pc=0x80094570;break;} // 93c10008
case 0x80094570:{r[30]=r[3]|r[3];c.pc=0x80094574;break;} // 7c7e1b78
case 0x80094574:{c.lr=0x80094578;c.pc=0x8005e0c0;break;} // 4bfc9b4d
case 0x80094578:{if(!word((r[3]+uint32_t(0)),r[0]))return {};c.pc=0x8009457c;break;} // 80030000
case 0x8009457c:{r[31]=r[3]|r[3];c.pc=0x80094580;break;} // 7c7f1b78
case 0x80094580:{r[0]=rotate(r[0],0)&0x10000000;compare(c,0,signedWord(r[0]),0);c.pc=0x80094584;break;} // 540000c7
case 0x80094584:{c.pc=(true)&&((c.condition[2]==false))?0x80094604:0x80094588;break;} // 40820080
case 0x80094588:{if(!half((r[31]+uint32_t(48)),r[0]))return {};c.pc=0x8009458c;break;} // a01f0030
case 0x8009458c:{compare(c,0,uint64_t(r[0]),16);c.pc=0x80094590;break;} // 28000010
case 0x80094590:{c.pc=(true)&&((c.condition[2]==true))?0x8009459c:0x80094594;break;} // 4182000c
case 0x80094594:{compare(c,0,uint64_t(r[0]),17);c.pc=0x80094598;break;} // 28000011
case 0x80094598:{c.pc=(true)&&((c.condition[2]==false))?0x800945c4:0x8009459c;break;} // 4082002c
case 0x8009459c:{if(!word((r[31]+uint32_t(4)),r[3]))return {};c.pc=0x800945a0;break;} // 807f0004
case 0x800945a0:{r[0]=rotate(r[3],0)&0x08000000;compare(c,0,signedWord(r[0]),0);c.pc=0x800945a4;break;} // 54600109
case 0x800945a4:{c.pc=(true)&&((c.condition[2]==true))?0x800945b0:0x800945a8;break;} // 4182000c
case 0x800945a8:{r[0]=rotate(r[3],0)&0xf7ffffff;c.pc=0x800945ac;break;} // 54600146
case 0x800945ac:{if(!store((r[31]+uint32_t(4)),r[0]))return {};c.pc=0x800945b0;break;} // 901f0004
case 0x800945b0:{if(!word((r[31]+uint32_t(4)),r[3]))return {};c.pc=0x800945b4;break;} // 807f0004
case 0x800945b4:{r[0]=rotate(r[3],0)&0x00000004;compare(c,0,signedWord(r[0]),0);c.pc=0x800945b8;break;} // 5460077b
case 0x800945b8:{c.pc=(true)&&((c.condition[2]==true))?0x800945c4:0x800945bc;break;} // 4182000c
case 0x800945bc:{r[0]=rotate(r[3],0)&0xfffffffb;c.pc=0x800945c0;break;} // 546007b8
case 0x800945c0:{if(!store((r[31]+uint32_t(4)),r[0]))return {};c.pc=0x800945c4;break;} // 901f0004
case 0x800945c4:{if(!half((r[31]+uint32_t(46)),r[0]))return {};c.pc=0x800945c8;break;} // a01f002e
case 0x800945c8:{r[3]=uint32_t(-2144272384);c.pc=0x800945cc;break;} // 3c608031
case 0x800945cc:{r[3]=r[3]+uint32_t(48);c.pc=0x800945d0;break;} // 38630030
case 0x800945d0:{if(!s.storeHalf||!s.storeHalf(s.user,(r[31]+uint32_t(48)),uint16_t(r[0])))return {};c.pc=0x800945d4;break;} // b01f0030
case 0x800945d4:{if(!s.storeHalf||!s.storeHalf(s.user,(r[31]+uint32_t(46)),uint16_t(r[30])))return {};c.pc=0x800945d8;break;} // b3df002e
case 0x800945d8:{if(!half((r[31]+uint32_t(48)),r[0]))return {};c.pc=0x800945dc;break;} // a01f0030
case 0x800945dc:{r[0]=r[0]*uint32_t(12);c.pc=0x800945e0;break;} // 1c00000c
case 0x800945e0:{r[3]=r[3]+r[0];c.pc=0x800945e4;break;} // 7c630214
case 0x800945e4:{if(!word((r[3]+uint32_t(8)),r[12]))return {};c.pc=0x800945e8;break;} // 81830008
case 0x800945e8:{compare(c,0,uint64_t(r[12]),0);c.pc=0x800945ec;break;} // 280c0000
case 0x800945ec:{c.pc=(true)&&((c.condition[2]==true))?0x800945f8:0x800945f0;break;} // 4182000c
case 0x800945f0:{c.ctr=r[12];c.pc=0x800945f4;break;} // 7d8903a6
case 0x800945f4:{if(c.ctr==0x8005b5c8||c.ctr==0x8005e0c0||c.ctr==0x8009455c||c.ctr==0x80095a4c||c.ctr==0x8009696c||c.ctr==0x800a6b10||c.ctr==0x800bd6ac||c.ctr==0x800d4ed4||c.ctr==0x800d50cc||c.ctr==0x800d59c8||c.ctr==0x8012f550||c.ctr==0x801a9488||c.ctr==0x801a9514||c.ctr==0x801a9544||c.ctr==0x801a95a8){c.lr=0x800945f8;c.pc=c.ctr;break;}if(!external(c.ctr,0x800945f8))return {};break;} // 4e800421
case 0x800945f8:{if(!word((r[31]+uint32_t(12)),r[0]))return {};c.pc=0x800945fc;break;} // 801f000c
case 0x800945fc:{r[0]=r[0]|0x00000001;c.pc=0x80094600;break;} // 60000001
case 0x80094600:{if(!store((r[31]+uint32_t(12)),r[0]))return {};c.pc=0x80094604;break;} // 901f000c
case 0x80094604:{if(!word((r[1]+uint32_t(20)),r[0]))return {};c.pc=0x80094608;break;} // 80010014
case 0x80094608:{if(!word((r[1]+uint32_t(12)),r[31]))return {};c.pc=0x8009460c;break;} // 83e1000c
case 0x8009460c:{if(!word((r[1]+uint32_t(8)),r[30]))return {};c.pc=0x80094610;break;} // 83c10008
case 0x80094610:{c.lr=r[0];c.pc=0x80094614;break;} // 7c0803a6
case 0x80094614:{r[1]=r[1]+uint32_t(16);c.pc=0x80094618;break;} // 38210010
case 0x80094618:{c.pc=c.lr;break;} // 4e800020
// mot_stay_post 800a6b10/ac SHA256 045f375d0d3f44bb67ed8d1b0e36f43a7e5d4d90a7a3fdc15e1477764c3b4376
case 0x800a6b10:{const auto target=(r[1]+uint32_t(-16));if(!store(target,r[1]))return {};r[1]=target;c.pc=0x800a6b14;break;} // 9421fff0
case 0x800a6b14:{r[0]=c.lr;c.pc=0x800a6b18;break;} // 7c0802a6
case 0x800a6b18:{if(!store((r[1]+uint32_t(20)),r[0]))return {};c.pc=0x800a6b1c;break;} // 90010014
case 0x800a6b1c:{if(!store((r[1]+uint32_t(12)),r[31]))return {};c.pc=0x800a6b20;break;} // 93e1000c
case 0x800a6b20:{c.lr=0x800a6b24;c.pc=0x8005e0c0;break;} // 4bfb75a1
case 0x800a6b24:{if(!half((r[3]+uint32_t(46)),r[0]))return {};c.pc=0x800a6b28;break;} // a003002e
case 0x800a6b28:{compare(c,0,uint64_t(r[0]),19);c.pc=0x800a6b2c;break;} // 28000013
case 0x800a6b2c:{c.pc=(true)&&((c.condition[2]==true))?0x800a6b84:0x800a6b30;break;} // 41820058
case 0x800a6b30:{compare(c,0,uint64_t(r[0]),20);c.pc=0x800a6b34;break;} // 28000014
case 0x800a6b34:{c.pc=(true)&&((c.condition[2]==true))?0x800a6b84:0x800a6b38;break;} // 41820050
case 0x800a6b38:{c.lr=0x800a6b3c;c.pc=0x8005e0c0;break;} // 4bfb7589
case 0x800a6b3c:{r[31]=r[3]|r[3];c.pc=0x800a6b40;break;} // 7c7f1b78
case 0x800a6b40:{if(!word((r[3]+uint32_t(636)),r[3]))return {};c.pc=0x800a6b44;break;} // 8063027c
case 0x800a6b44:{r[0]=r[3]+uint32_t(65536);c.pc=0x800a6b48;break;} // 3c030001
case 0x800a6b48:{compare(c,0,uint64_t(r[0]),65535);c.pc=0x800a6b4c;break;} // 2800ffff
case 0x800a6b4c:{c.pc=(true)&&((c.condition[2]==true))?0x800a6b5c:0x800a6b50;break;} // 41820010
case 0x800a6b50:{if(!external(0x800d95f8,0x800a6b54))return {};break;} // 48032aa9
case 0x800a6b54:{r[0]=uint32_t(-1);c.pc=0x800a6b58;break;} // 3800ffff
case 0x800a6b58:{if(!store((r[31]+uint32_t(636)),r[0]))return {};c.pc=0x800a6b5c;break;} // 901f027c
case 0x800a6b5c:{c.lr=0x800a6b60;c.pc=0x8005e0c0;break;} // 4bfb7565
case 0x800a6b60:{r[31]=r[3]|r[3];c.pc=0x800a6b64;break;} // 7c7f1b78
case 0x800a6b64:{if(!word((r[3]+uint32_t(640)),r[3]))return {};c.pc=0x800a6b68;break;} // 80630280
case 0x800a6b68:{r[0]=r[3]+uint32_t(65536);c.pc=0x800a6b6c;break;} // 3c030001
case 0x800a6b6c:{compare(c,0,uint64_t(r[0]),65535);c.pc=0x800a6b70;break;} // 2800ffff
case 0x800a6b70:{c.pc=(true)&&((c.condition[2]==true))?0x800a6ba8:0x800a6b74;break;} // 41820038
case 0x800a6b74:{if(!external(0x800d95f8,0x800a6b78))return {};break;} // 48032a85
case 0x800a6b78:{r[0]=uint32_t(-1);c.pc=0x800a6b7c;break;} // 3800ffff
case 0x800a6b7c:{if(!store((r[31]+uint32_t(640)),r[0]))return {};c.pc=0x800a6b80;break;} // 901f0280
case 0x800a6b80:{c.pc=0x800a6ba8;break;} // 48000028
case 0x800a6b84:{c.lr=0x800a6b88;c.pc=0x8005e0c0;break;} // 4bfb753d
case 0x800a6b88:{r[31]=r[3]|r[3];c.pc=0x800a6b8c;break;} // 7c7f1b78
case 0x800a6b8c:{if(!word((r[3]+uint32_t(636)),r[3]))return {};c.pc=0x800a6b90;break;} // 8063027c
case 0x800a6b90:{r[0]=r[3]+uint32_t(65536);c.pc=0x800a6b94;break;} // 3c030001
case 0x800a6b94:{compare(c,0,uint64_t(r[0]),65535);c.pc=0x800a6b98;break;} // 2800ffff
case 0x800a6b98:{c.pc=(true)&&((c.condition[2]==true))?0x800a6ba8:0x800a6b9c;break;} // 41820010
case 0x800a6b9c:{if(!external(0x800d95f8,0x800a6ba0))return {};break;} // 48032a5d
case 0x800a6ba0:{r[0]=uint32_t(-1);c.pc=0x800a6ba4;break;} // 3800ffff
case 0x800a6ba4:{if(!store((r[31]+uint32_t(636)),r[0]))return {};c.pc=0x800a6ba8;break;} // 901f027c
case 0x800a6ba8:{if(!word((r[1]+uint32_t(20)),r[0]))return {};c.pc=0x800a6bac;break;} // 80010014
case 0x800a6bac:{if(!word((r[1]+uint32_t(12)),r[31]))return {};c.pc=0x800a6bb0;break;} // 83e1000c
case 0x800a6bb0:{c.lr=r[0];c.pc=0x800a6bb4;break;} // 7c0803a6
case 0x800a6bb4:{r[1]=r[1]+uint32_t(16);c.pc=0x800a6bb8;break;} // 38210010
case 0x800a6bb8:{c.pc=c.lr;break;} // 4e800020
// marioResetCamFollowRate 801a9488/38 SHA256 65f264bf8e3ea2750a60552965dbfe582ac0fcc189552b0d4b357ff6ad8f95fe
case 0x801a9488:{const auto target=(r[1]+uint32_t(-32));if(!store(target,r[1]))return {};r[1]=target;c.pc=0x801a948c;break;} // 9421ffe0
case 0x801a948c:{r[0]=c.lr;c.pc=0x801a9490;break;} // 7c0802a6
case 0x801a9490:{if(!store((r[1]+uint32_t(36)),r[0]))return {};c.pc=0x801a9494;break;} // 90010024
case 0x801a9494:{const auto value=doubleBits(double(scalar(f[31])));while(c.step<2){if(!store((r[1]+uint32_t(16))+4*c.step,uint32_t(value>>(c.step?0:32))))return {};++c.step;}c.step=0;c.pc=0x801a9498;break;} // dbe10010
case 0x801a9498:{while(c.step<2){if(!store((r[1]+uint32_t(24))+4*c.step,c.step?c.paired[31]:f[31]))return {};++c.step;}c.step=0;c.pc=0x801a949c;break;} // f3e10018
case 0x801a949c:{if(!word((r[2]+uint32_t(-10596)),f[31]))return {};c.pc=0x801a94a0;break;} // c3e2d69c
case 0x801a94a0:{c.lr=0x801a94a4;c.pc=0x8005e0c0;break;} // 4beb4c21
case 0x801a94a4:{if(!store((r[3]+uint32_t(348)),f[31]))return {};c.pc=0x801a94a8;break;} // d3e3015c
case 0x801a94a8:{if(!c.step){if(!word((r[1]+uint32_t(24)),f[31]))return {};c.step=1;}if(!word((r[1]+uint32_t(24))+4,c.paired[31]))return {};c.step=0;c.pc=0x801a94ac;break;} // e3e10018
case 0x801a94ac:{if(!word((r[1]+uint32_t(36)),r[0]))return {};c.pc=0x801a94b0;break;} // 80010024
case 0x801a94b0:{if(!c.step){if(!word((r[1]+uint32_t(16)),c.transfer))return {};c.step=1;}uint32_t low;if(!word((r[1]+uint32_t(16))+4,low))return {};f[31]=bits(float(doubleScalar(uint64_t(c.transfer)<<32|low)));c.step=0;c.pc=0x801a94b4;break;} // cbe10010
case 0x801a94b4:{c.lr=r[0];c.pc=0x801a94b8;break;} // 7c0803a6
case 0x801a94b8:{r[1]=r[1]+uint32_t(32);c.pc=0x801a94bc;break;} // 38210020
case 0x801a94bc:{c.pc=c.lr;break;} // 4e800020
// marioInitCamId 801a9514/30 SHA256 3bb71c8239e37aa8973bef56fa964f5a78ad4354f2b848e5808d35633a385965
case 0x801a9514:{const auto target=(r[1]+uint32_t(-16));if(!store(target,r[1]))return {};r[1]=target;c.pc=0x801a9518;break;} // 9421fff0
case 0x801a9518:{r[0]=c.lr;c.pc=0x801a951c;break;} // 7c0802a6
case 0x801a951c:{if(!store((r[1]+uint32_t(20)),r[0]))return {};c.pc=0x801a9520;break;} // 90010014
case 0x801a9520:{if(!store((r[1]+uint32_t(12)),r[31]))return {};c.pc=0x801a9524;break;} // 93e1000c
case 0x801a9524:{r[31]=uint32_t(4);c.pc=0x801a9528;break;} // 3be00004
case 0x801a9528:{c.lr=0x801a952c;c.pc=0x8005e0c0;break;} // 4beb4b99
case 0x801a952c:{if(!store((r[3]+uint32_t(308)),r[31]))return {};c.pc=0x801a9530;break;} // 93e30134
case 0x801a9530:{if(!word((r[1]+uint32_t(20)),r[0]))return {};c.pc=0x801a9534;break;} // 80010014
case 0x801a9534:{if(!word((r[1]+uint32_t(12)),r[31]))return {};c.pc=0x801a9538;break;} // 83e1000c
case 0x801a9538:{c.lr=r[0];c.pc=0x801a953c;break;} // 7c0803a6
case 0x801a953c:{r[1]=r[1]+uint32_t(16);c.pc=0x801a9540;break;} // 38210010
case 0x801a9540:{c.pc=c.lr;break;} // 4e800020
// marioResetCamShiftRate 801a9544/64 SHA256 3d2142763b6d9f0adba6fbbb64ee5e4198b4a78de4037304fd0875c85e0a0ee5
case 0x801a9544:{const auto target=(r[1]+uint32_t(-16));if(!store(target,r[1]))return {};r[1]=target;c.pc=0x801a9548;break;} // 9421fff0
case 0x801a9548:{r[0]=c.lr;c.pc=0x801a954c;break;} // 7c0802a6
case 0x801a954c:{if(!store((r[1]+uint32_t(20)),r[0]))return {};c.pc=0x801a9550;break;} // 90010014
case 0x801a9550:{c.lr=0x801a9554;c.pc=0x8005e0c0;break;} // 4beb4b71
case 0x801a9554:{if(!word((r[2]+uint32_t(-10592)),f[0]))return {};c.pc=0x801a9558;break;} // c002d6a0
case 0x801a9558:{r[4]=uint32_t(-2144337920);c.pc=0x801a955c;break;} // 3c808030
case 0x801a955c:{if(!word((r[2]+uint32_t(-10588)),f[2]))return {};c.pc=0x801a9560;break;} // c042d6a4
case 0x801a9560:{r[5]=r[4]+uint32_t(-22700);c.pc=0x801a9564;break;} // 38a4a754
case 0x801a9564:{if(!store((r[3]+uint32_t(336)),f[0]))return {};c.pc=0x801a9568;break;} // d0030150
case 0x801a9568:{if(!word((r[2]+uint32_t(-10652)),f[1]))return {};c.pc=0x801a956c;break;} // c022d664
case 0x801a956c:{if(!store((r[3]+uint32_t(340)),f[2]))return {};c.pc=0x801a9570;break;} // d0430154
case 0x801a9570:{if(!word((r[2]+uint32_t(-10584)),f[0]))return {};c.pc=0x801a9574;break;} // c002d6a8
case 0x801a9574:{if(!store((r[3]+uint32_t(344)),f[2]))return {};c.pc=0x801a9578;break;} // d0430158
case 0x801a9578:{if(!store((r[3]+uint32_t(100)),f[1]))return {};c.pc=0x801a957c;break;} // d0230064
case 0x801a957c:{if(!store((r[3]+uint32_t(104)),f[0]))return {};c.pc=0x801a9580;break;} // d0030068
case 0x801a9580:{if(!word((r[5]+uint32_t(0)),r[4]))return {};c.pc=0x801a9584;break;} // 80850000
case 0x801a9584:{if(!word((r[5]+uint32_t(4)),r[0]))return {};c.pc=0x801a9588;break;} // 80050004
case 0x801a9588:{if(!store((r[3]+uint32_t(108)),r[4]))return {};c.pc=0x801a958c;break;} // 9083006c
case 0x801a958c:{if(!store((r[3]+uint32_t(112)),r[0]))return {};c.pc=0x801a9590;break;} // 90030070
case 0x801a9590:{if(!word((r[5]+uint32_t(8)),r[0]))return {};c.pc=0x801a9594;break;} // 80050008
case 0x801a9594:{if(!store((r[3]+uint32_t(116)),r[0]))return {};c.pc=0x801a9598;break;} // 90030074
case 0x801a9598:{if(!word((r[1]+uint32_t(20)),r[0]))return {};c.pc=0x801a959c;break;} // 80010014
case 0x801a959c:{c.lr=r[0];c.pc=0x801a95a0;break;} // 7c0803a6
case 0x801a95a0:{r[1]=r[1]+uint32_t(16);c.pc=0x801a95a4;break;} // 38210010
case 0x801a95a4:{c.pc=c.lr;break;} // 4e800020
// marioResetCamShift 801a95a8/5c SHA256 52ac82ec041e67cecd8d7b0c9dc3e9a0ef6c4236c930256d1f2368f5824fa272
case 0x801a95a8:{const auto target=(r[1]+uint32_t(-16));if(!store(target,r[1]))return {};r[1]=target;c.pc=0x801a95ac;break;} // 9421fff0
case 0x801a95ac:{r[0]=c.lr;c.pc=0x801a95b0;break;} // 7c0802a6
case 0x801a95b0:{if(!store((r[1]+uint32_t(20)),r[0]))return {};c.pc=0x801a95b4;break;} // 90010014
case 0x801a95b4:{c.lr=0x801a95b8;c.pc=0x8005e0c0;break;} // 4beb4b0d
case 0x801a95b8:{if(!word((r[2]+uint32_t(-10652)),f[1]))return {};c.pc=0x801a95bc;break;} // c022d664
case 0x801a95bc:{r[4]=uint32_t(-2144337920);c.pc=0x801a95c0;break;} // 3c808030
case 0x801a95c0:{if(!word((r[2]+uint32_t(-10584)),f[0]))return {};c.pc=0x801a95c4;break;} // c002d6a8
case 0x801a95c4:{r[5]=r[4]+uint32_t(-22712);c.pc=0x801a95c8;break;} // 38a4a748
case 0x801a95c8:{if(!store((r[3]+uint32_t(312)),f[1]))return {};c.pc=0x801a95cc;break;} // d0230138
case 0x801a95cc:{if(!store((r[3]+uint32_t(320)),f[1]))return {};c.pc=0x801a95d0;break;} // d0230140
case 0x801a95d0:{if(!store((r[3]+uint32_t(328)),f[1]))return {};c.pc=0x801a95d4;break;} // d0230148
case 0x801a95d4:{if(!store((r[3]+uint32_t(100)),f[1]))return {};c.pc=0x801a95d8;break;} // d0230064
case 0x801a95d8:{if(!store((r[3]+uint32_t(104)),f[0]))return {};c.pc=0x801a95dc;break;} // d0030068
case 0x801a95dc:{if(!word((r[5]+uint32_t(0)),r[4]))return {};c.pc=0x801a95e0;break;} // 80850000
case 0x801a95e0:{if(!word((r[5]+uint32_t(4)),r[0]))return {};c.pc=0x801a95e4;break;} // 80050004
case 0x801a95e4:{if(!store((r[3]+uint32_t(108)),r[4]))return {};c.pc=0x801a95e8;break;} // 9083006c
case 0x801a95e8:{if(!store((r[3]+uint32_t(112)),r[0]))return {};c.pc=0x801a95ec;break;} // 90030070
case 0x801a95ec:{if(!word((r[5]+uint32_t(8)),r[0]))return {};c.pc=0x801a95f0;break;} // 80050008
case 0x801a95f0:{if(!store((r[3]+uint32_t(116)),r[0]))return {};c.pc=0x801a95f4;break;} // 90030074
case 0x801a95f4:{if(!word((r[1]+uint32_t(20)),r[0]))return {};c.pc=0x801a95f8;break;} // 80010014
case 0x801a95f8:{c.lr=r[0];c.pc=0x801a95fc;break;} // 7c0803a6
case 0x801a95fc:{r[1]=r[1]+uint32_t(16);c.pc=0x801a9600;break;} // 38210010
case 0x801a9600:{c.pc=c.lr;break;} // 4e800020
// marioGetPtr 8005e0c0/8 SHA256 3a59459f28ddd5d287c200434ad158a9dfe277f4f6bcaae3eb8698505aa9773d
case 0x8005e0c0:{if(!word((r[13]+uint32_t(6624)),r[3]))return {};c.pc=0x8005e0c4;break;} // 806d19e0
case 0x8005e0c4:{c.pc=c.lr;break;} // 4e800020
// pouchInit 800d59c8/4e8 SHA256 c6e29f88e574c1d5caa1aaf5c7a73a53bc2d952cbad00a9e21ab2bfd3113def1
case 0x800d59c8:{const auto target=(r[1]+uint32_t(-16));if(!store(target,r[1]))return {};r[1]=target;c.pc=0x800d59cc;break;} // 9421fff0
case 0x800d59cc:{r[0]=c.lr;c.pc=0x800d59d0;break;} // 7c0802a6
case 0x800d59d0:{r[3]=uint32_t(0);c.pc=0x800d59d4;break;} // 38600000
case 0x800d59d4:{r[4]=uint32_t(1492);c.pc=0x800d59d8;break;} // 388005d4
case 0x800d59d8:{if(!store((r[1]+uint32_t(20)),r[0]))return {};c.pc=0x800d59dc;break;} // 90010014
case 0x800d59dc:{if(!external(0x800300f0,0x800d59e0))return {};break;} // 4bf5a715
case 0x800d59e0:{if(!store((r[13]+uint32_t(7136)),r[3]))return {};c.pc=0x800d59e4;break;} // 906d1be0
case 0x800d59e4:{r[4]=uint32_t(0);c.pc=0x800d59e8;break;} // 38800000
case 0x800d59e8:{r[5]=uint32_t(1492);c.pc=0x800d59ec;break;} // 38a005d4
case 0x800d59ec:{if(!external(0x800050b4,0x800d59f0))return {};break;} // 4bf2f6c9
case 0x800d59f0:{r[0]=uint32_t(15);c.pc=0x800d59f4;break;} // 3800000f
case 0x800d59f4:{r[12]=uint32_t(0);c.pc=0x800d59f8;break;} // 39800000
case 0x800d59f8:{r[11]=uint32_t(0);c.pc=0x800d59fc;break;} // 39600000
case 0x800d59fc:{c.ctr=r[0];c.pc=0x800d5a00;break;} // 7c0903a6
case 0x800d5a00:{if(!word((r[13]+uint32_t(7136)),r[3]))return {};c.pc=0x800d5a04;break;} // 806d1be0
case 0x800d5a04:{r[0]=r[11]+uint32_t(160);c.pc=0x800d5a08;break;} // 380b00a0
case 0x800d5a08:{r[10]=uint32_t(0);c.pc=0x800d5a0c;break;} // 39400000
case 0x800d5a0c:{r[8]=r[11]+uint32_t(162);c.pc=0x800d5a10;break;} // 390b00a2
case 0x800d5a10:{if(!s.storeHalf||!s.storeHalf(s.user,r[3]+r[0],uint16_t(r[10])))return {};c.pc=0x800d5a14;break;} // 7d43032e
case 0x800d5a14:{r[7]=r[11]+uint32_t(164);c.pc=0x800d5a18;break;} // 38eb00a4
case 0x800d5a18:{r[6]=r[11]+uint32_t(166);c.pc=0x800d5a1c;break;} // 38cb00a6
case 0x800d5a1c:{r[5]=r[11]+uint32_t(168);c.pc=0x800d5a20;break;} // 38ab00a8
case 0x800d5a20:{if(!word((r[13]+uint32_t(7136)),r[9]))return {};c.pc=0x800d5a24;break;} // 812d1be0
case 0x800d5a24:{r[4]=r[11]+uint32_t(170);c.pc=0x800d5a28;break;} // 388b00aa
case 0x800d5a28:{r[3]=r[11]+uint32_t(172);c.pc=0x800d5a2c;break;} // 386b00ac
case 0x800d5a2c:{r[0]=r[11]+uint32_t(174);c.pc=0x800d5a30;break;} // 380b00ae
case 0x800d5a30:{if(!s.storeHalf||!s.storeHalf(s.user,r[9]+r[8],uint16_t(r[10])))return {};c.pc=0x800d5a34;break;} // 7d49432e
case 0x800d5a34:{r[11]=r[11]+uint32_t(16);c.pc=0x800d5a38;break;} // 396b0010
case 0x800d5a38:{r[12]=r[12]+uint32_t(8);c.pc=0x800d5a3c;break;} // 398c0008
case 0x800d5a3c:{if(!word((r[13]+uint32_t(7136)),r[8]))return {};c.pc=0x800d5a40;break;} // 810d1be0
case 0x800d5a40:{if(!s.storeHalf||!s.storeHalf(s.user,r[8]+r[7],uint16_t(r[10])))return {};c.pc=0x800d5a44;break;} // 7d483b2e
case 0x800d5a44:{if(!word((r[13]+uint32_t(7136)),r[7]))return {};c.pc=0x800d5a48;break;} // 80ed1be0
case 0x800d5a48:{if(!s.storeHalf||!s.storeHalf(s.user,r[7]+r[6],uint16_t(r[10])))return {};c.pc=0x800d5a4c;break;} // 7d47332e
case 0x800d5a4c:{if(!word((r[13]+uint32_t(7136)),r[6]))return {};c.pc=0x800d5a50;break;} // 80cd1be0
case 0x800d5a50:{if(!s.storeHalf||!s.storeHalf(s.user,r[6]+r[5],uint16_t(r[10])))return {};c.pc=0x800d5a54;break;} // 7d462b2e
case 0x800d5a54:{if(!word((r[13]+uint32_t(7136)),r[5]))return {};c.pc=0x800d5a58;break;} // 80ad1be0
case 0x800d5a58:{if(!s.storeHalf||!s.storeHalf(s.user,r[5]+r[4],uint16_t(r[10])))return {};c.pc=0x800d5a5c;break;} // 7d45232e
case 0x800d5a5c:{if(!word((r[13]+uint32_t(7136)),r[4]))return {};c.pc=0x800d5a60;break;} // 808d1be0
case 0x800d5a60:{if(!s.storeHalf||!s.storeHalf(s.user,r[4]+r[3],uint16_t(r[10])))return {};c.pc=0x800d5a64;break;} // 7d441b2e
case 0x800d5a64:{if(!word((r[13]+uint32_t(7136)),r[3]))return {};c.pc=0x800d5a68;break;} // 806d1be0
case 0x800d5a68:{if(!s.storeHalf||!s.storeHalf(s.user,r[3]+r[0],uint16_t(r[10])))return {};c.pc=0x800d5a6c;break;} // 7d43032e
case 0x800d5a6c:{--c.ctr;c.pc=(c.ctr!=0)&&(true)?0x800d5a00:0x800d5a70;break;} // 4200ff94
case 0x800d5a70:{const uint64_t value=uint64_t(uint32_t(121))+uint64_t(~r[12])+1;c.carry=value>>32;r[0]=uint32_t(value);c.pc=0x800d5a74;break;} // 200c0079
case 0x800d5a74:{r[4]=rotate(r[12],1)&0xfffffffe;c.pc=0x800d5a78;break;} // 5584083c
case 0x800d5a78:{c.ctr=r[0];c.pc=0x800d5a7c;break;} // 7c0903a6
case 0x800d5a7c:{compare(c,0,signedWord(r[12]),121);c.pc=0x800d5a80;break;} // 2c0c0079
case 0x800d5a80:{c.pc=(true)&&((c.condition[0]==false))?0x800d5a98:0x800d5a84;break;} // 40800018
case 0x800d5a84:{if(!word((r[13]+uint32_t(7136)),r[3]))return {};c.pc=0x800d5a88;break;} // 806d1be0
case 0x800d5a88:{r[0]=r[4]+uint32_t(160);c.pc=0x800d5a8c;break;} // 380400a0
case 0x800d5a8c:{r[4]=r[4]+uint32_t(2);c.pc=0x800d5a90;break;} // 38840002
case 0x800d5a90:{if(!s.storeHalf||!s.storeHalf(s.user,r[3]+r[0],uint16_t(r[10])))return {};c.pc=0x800d5a94;break;} // 7d43032e
case 0x800d5a94:{--c.ctr;c.pc=(c.ctr!=0)&&(true)?0x800d5a84:0x800d5a98;break;} // 4200fff0
case 0x800d5a98:{if(!word((r[13]+uint32_t(7136)),r[3]))return {};c.pc=0x800d5a9c;break;} // 806d1be0
case 0x800d5a9c:{r[12]=uint32_t(0);c.pc=0x800d5aa0;break;} // 39800000
case 0x800d5aa0:{r[0]=uint32_t(2);c.pc=0x800d5aa4;break;} // 38000002
case 0x800d5aa4:{if(!s.storeHalf||!s.storeHalf(s.user,(r[3]+uint32_t(402)),uint16_t(r[12])))return {};c.pc=0x800d5aa8;break;} // b1830192
case 0x800d5aa8:{if(!word((r[13]+uint32_t(7136)),r[3]))return {};c.pc=0x800d5aac;break;} // 806d1be0
case 0x800d5aac:{if(!s.storeHalf||!s.storeHalf(s.user,(r[3]+uint32_t(404)),uint16_t(r[12])))return {};c.pc=0x800d5ab0;break;} // b1830194
case 0x800d5ab0:{if(!word((r[13]+uint32_t(7136)),r[3]))return {};c.pc=0x800d5ab4;break;} // 806d1be0
case 0x800d5ab4:{if(!s.storeHalf||!s.storeHalf(s.user,(r[3]+uint32_t(406)),uint16_t(r[12])))return {};c.pc=0x800d5ab8;break;} // b1830196
case 0x800d5ab8:{if(!word((r[13]+uint32_t(7136)),r[3]))return {};c.pc=0x800d5abc;break;} // 806d1be0
case 0x800d5abc:{if(!s.storeHalf||!s.storeHalf(s.user,(r[3]+uint32_t(408)),uint16_t(r[12])))return {};c.pc=0x800d5ac0;break;} // b1830198
case 0x800d5ac0:{if(!word((r[13]+uint32_t(7136)),r[3]))return {};c.pc=0x800d5ac4;break;} // 806d1be0
case 0x800d5ac4:{if(!s.storeHalf||!s.storeHalf(s.user,(r[3]+uint32_t(410)),uint16_t(r[12])))return {};c.pc=0x800d5ac8;break;} // b183019a
case 0x800d5ac8:{if(!word((r[13]+uint32_t(7136)),r[3]))return {};c.pc=0x800d5acc;break;} // 806d1be0
case 0x800d5acc:{if(!s.storeHalf||!s.storeHalf(s.user,(r[3]+uint32_t(412)),uint16_t(r[12])))return {};c.pc=0x800d5ad0;break;} // b183019c
case 0x800d5ad0:{if(!word((r[13]+uint32_t(7136)),r[3]))return {};c.pc=0x800d5ad4;break;} // 806d1be0
case 0x800d5ad4:{if(!s.storeHalf||!s.storeHalf(s.user,(r[3]+uint32_t(414)),uint16_t(r[12])))return {};c.pc=0x800d5ad8;break;} // b183019e
case 0x800d5ad8:{if(!word((r[13]+uint32_t(7136)),r[3]))return {};c.pc=0x800d5adc;break;} // 806d1be0
case 0x800d5adc:{if(!s.storeHalf||!s.storeHalf(s.user,(r[3]+uint32_t(416)),uint16_t(r[12])))return {};c.pc=0x800d5ae0;break;} // b18301a0
case 0x800d5ae0:{if(!word((r[13]+uint32_t(7136)),r[3]))return {};c.pc=0x800d5ae4;break;} // 806d1be0
case 0x800d5ae4:{if(!s.storeHalf||!s.storeHalf(s.user,(r[3]+uint32_t(418)),uint16_t(r[12])))return {};c.pc=0x800d5ae8;break;} // b18301a2
case 0x800d5ae8:{if(!word((r[13]+uint32_t(7136)),r[3]))return {};c.pc=0x800d5aec;break;} // 806d1be0
case 0x800d5aec:{if(!s.storeHalf||!s.storeHalf(s.user,(r[3]+uint32_t(420)),uint16_t(r[12])))return {};c.pc=0x800d5af0;break;} // b18301a4
case 0x800d5af0:{if(!word((r[13]+uint32_t(7136)),r[3]))return {};c.pc=0x800d5af4;break;} // 806d1be0
case 0x800d5af4:{if(!s.storeHalf||!s.storeHalf(s.user,(r[3]+uint32_t(422)),uint16_t(r[12])))return {};c.pc=0x800d5af8;break;} // b18301a6
case 0x800d5af8:{if(!word((r[13]+uint32_t(7136)),r[3]))return {};c.pc=0x800d5afc;break;} // 806d1be0
case 0x800d5afc:{if(!s.storeHalf||!s.storeHalf(s.user,(r[3]+uint32_t(424)),uint16_t(r[12])))return {};c.pc=0x800d5b00;break;} // b18301a8
case 0x800d5b00:{if(!word((r[13]+uint32_t(7136)),r[3]))return {};c.pc=0x800d5b04;break;} // 806d1be0
case 0x800d5b04:{if(!s.storeHalf||!s.storeHalf(s.user,(r[3]+uint32_t(426)),uint16_t(r[12])))return {};c.pc=0x800d5b08;break;} // b18301aa
case 0x800d5b08:{if(!word((r[13]+uint32_t(7136)),r[3]))return {};c.pc=0x800d5b0c;break;} // 806d1be0
case 0x800d5b0c:{if(!s.storeHalf||!s.storeHalf(s.user,(r[3]+uint32_t(428)),uint16_t(r[12])))return {};c.pc=0x800d5b10;break;} // b18301ac
case 0x800d5b10:{if(!word((r[13]+uint32_t(7136)),r[3]))return {};c.pc=0x800d5b14;break;} // 806d1be0
case 0x800d5b14:{if(!s.storeHalf||!s.storeHalf(s.user,(r[3]+uint32_t(430)),uint16_t(r[12])))return {};c.pc=0x800d5b18;break;} // b18301ae
case 0x800d5b18:{if(!word((r[13]+uint32_t(7136)),r[3]))return {};c.pc=0x800d5b1c;break;} // 806d1be0
case 0x800d5b1c:{if(!s.storeHalf||!s.storeHalf(s.user,(r[3]+uint32_t(432)),uint16_t(r[12])))return {};c.pc=0x800d5b20;break;} // b18301b0
case 0x800d5b20:{if(!word((r[13]+uint32_t(7136)),r[3]))return {};c.pc=0x800d5b24;break;} // 806d1be0
case 0x800d5b24:{if(!s.storeHalf||!s.storeHalf(s.user,(r[3]+uint32_t(434)),uint16_t(r[12])))return {};c.pc=0x800d5b28;break;} // b18301b2
case 0x800d5b28:{if(!word((r[13]+uint32_t(7136)),r[3]))return {};c.pc=0x800d5b2c;break;} // 806d1be0
case 0x800d5b2c:{if(!s.storeHalf||!s.storeHalf(s.user,(r[3]+uint32_t(436)),uint16_t(r[12])))return {};c.pc=0x800d5b30;break;} // b18301b4
case 0x800d5b30:{if(!word((r[13]+uint32_t(7136)),r[3]))return {};c.pc=0x800d5b34;break;} // 806d1be0
case 0x800d5b34:{if(!s.storeHalf||!s.storeHalf(s.user,(r[3]+uint32_t(438)),uint16_t(r[12])))return {};c.pc=0x800d5b38;break;} // b18301b6
case 0x800d5b38:{if(!word((r[13]+uint32_t(7136)),r[3]))return {};c.pc=0x800d5b3c;break;} // 806d1be0
case 0x800d5b3c:{if(!s.storeHalf||!s.storeHalf(s.user,(r[3]+uint32_t(440)),uint16_t(r[12])))return {};c.pc=0x800d5b40;break;} // b18301b8
case 0x800d5b40:{c.ctr=r[0];c.pc=0x800d5b44;break;} // 7c0903a6
case 0x800d5b44:{if(!word((r[13]+uint32_t(7136)),r[4]))return {};c.pc=0x800d5b48;break;} // 808d1be0
case 0x800d5b48:{r[11]=r[12]+uint32_t(442);c.pc=0x800d5b4c;break;} // 396c01ba
case 0x800d5b4c:{r[3]=uint32_t(0);c.pc=0x800d5b50;break;} // 38600000
case 0x800d5b50:{r[10]=r[12]+uint32_t(444);c.pc=0x800d5b54;break;} // 394c01bc
case 0x800d5b54:{if(!s.storeHalf||!s.storeHalf(s.user,r[4]+r[11],uint16_t(r[3])))return {};c.pc=0x800d5b58;break;} // 7c645b2e
case 0x800d5b58:{r[9]=r[12]+uint32_t(446);c.pc=0x800d5b5c;break;} // 392c01be
case 0x800d5b5c:{r[8]=r[12]+uint32_t(448);c.pc=0x800d5b60;break;} // 390c01c0
case 0x800d5b60:{r[7]=r[12]+uint32_t(450);c.pc=0x800d5b64;break;} // 38ec01c2
case 0x800d5b64:{if(!word((r[13]+uint32_t(7136)),r[4]))return {};c.pc=0x800d5b68;break;} // 808d1be0
case 0x800d5b68:{r[6]=r[12]+uint32_t(452);c.pc=0x800d5b6c;break;} // 38cc01c4
case 0x800d5b6c:{r[5]=r[12]+uint32_t(454);c.pc=0x800d5b70;break;} // 38ac01c6
case 0x800d5b70:{r[0]=r[12]+uint32_t(456);c.pc=0x800d5b74;break;} // 380c01c8
case 0x800d5b74:{if(!s.storeHalf||!s.storeHalf(s.user,r[4]+r[10],uint16_t(r[3])))return {};c.pc=0x800d5b78;break;} // 7c64532e
case 0x800d5b78:{r[11]=r[12]+uint32_t(458);c.pc=0x800d5b7c;break;} // 396c01ca
case 0x800d5b7c:{r[10]=r[12]+uint32_t(460);c.pc=0x800d5b80;break;} // 394c01cc
case 0x800d5b80:{if(!word((r[13]+uint32_t(7136)),r[4]))return {};c.pc=0x800d5b84;break;} // 808d1be0
case 0x800d5b84:{if(!s.storeHalf||!s.storeHalf(s.user,r[4]+r[9],uint16_t(r[3])))return {};c.pc=0x800d5b88;break;} // 7c644b2e
case 0x800d5b88:{r[9]=r[12]+uint32_t(462);c.pc=0x800d5b8c;break;} // 392c01ce
case 0x800d5b8c:{if(!word((r[13]+uint32_t(7136)),r[4]))return {};c.pc=0x800d5b90;break;} // 808d1be0
case 0x800d5b90:{if(!s.storeHalf||!s.storeHalf(s.user,r[4]+r[8],uint16_t(r[3])))return {};c.pc=0x800d5b94;break;} // 7c64432e
case 0x800d5b94:{r[8]=r[12]+uint32_t(464);c.pc=0x800d5b98;break;} // 390c01d0
case 0x800d5b98:{if(!word((r[13]+uint32_t(7136)),r[4]))return {};c.pc=0x800d5b9c;break;} // 808d1be0
case 0x800d5b9c:{if(!s.storeHalf||!s.storeHalf(s.user,r[4]+r[7],uint16_t(r[3])))return {};c.pc=0x800d5ba0;break;} // 7c643b2e
case 0x800d5ba0:{r[7]=r[12]+uint32_t(466);c.pc=0x800d5ba4;break;} // 38ec01d2
case 0x800d5ba4:{if(!word((r[13]+uint32_t(7136)),r[4]))return {};c.pc=0x800d5ba8;break;} // 808d1be0
case 0x800d5ba8:{if(!s.storeHalf||!s.storeHalf(s.user,r[4]+r[6],uint16_t(r[3])))return {};c.pc=0x800d5bac;break;} // 7c64332e
case 0x800d5bac:{r[6]=r[12]+uint32_t(468);c.pc=0x800d5bb0;break;} // 38cc01d4
case 0x800d5bb0:{if(!word((r[13]+uint32_t(7136)),r[4]))return {};c.pc=0x800d5bb4;break;} // 808d1be0
case 0x800d5bb4:{if(!s.storeHalf||!s.storeHalf(s.user,r[4]+r[5],uint16_t(r[3])))return {};c.pc=0x800d5bb8;break;} // 7c642b2e
case 0x800d5bb8:{r[5]=r[12]+uint32_t(470);c.pc=0x800d5bbc;break;} // 38ac01d6
case 0x800d5bbc:{if(!word((r[13]+uint32_t(7136)),r[4]))return {};c.pc=0x800d5bc0;break;} // 808d1be0
case 0x800d5bc0:{if(!s.storeHalf||!s.storeHalf(s.user,r[4]+r[0],uint16_t(r[3])))return {};c.pc=0x800d5bc4;break;} // 7c64032e
case 0x800d5bc4:{r[0]=r[12]+uint32_t(472);c.pc=0x800d5bc8;break;} // 380c01d8
case 0x800d5bc8:{r[12]=r[12]+uint32_t(32);c.pc=0x800d5bcc;break;} // 398c0020
case 0x800d5bcc:{if(!word((r[13]+uint32_t(7136)),r[4]))return {};c.pc=0x800d5bd0;break;} // 808d1be0
case 0x800d5bd0:{if(!s.storeHalf||!s.storeHalf(s.user,r[4]+r[11],uint16_t(r[3])))return {};c.pc=0x800d5bd4;break;} // 7c645b2e
case 0x800d5bd4:{if(!word((r[13]+uint32_t(7136)),r[4]))return {};c.pc=0x800d5bd8;break;} // 808d1be0
case 0x800d5bd8:{if(!s.storeHalf||!s.storeHalf(s.user,r[4]+r[10],uint16_t(r[3])))return {};c.pc=0x800d5bdc;break;} // 7c64532e
case 0x800d5bdc:{if(!word((r[13]+uint32_t(7136)),r[4]))return {};c.pc=0x800d5be0;break;} // 808d1be0
case 0x800d5be0:{if(!s.storeHalf||!s.storeHalf(s.user,r[4]+r[9],uint16_t(r[3])))return {};c.pc=0x800d5be4;break;} // 7c644b2e
case 0x800d5be4:{if(!word((r[13]+uint32_t(7136)),r[4]))return {};c.pc=0x800d5be8;break;} // 808d1be0
case 0x800d5be8:{if(!s.storeHalf||!s.storeHalf(s.user,r[4]+r[8],uint16_t(r[3])))return {};c.pc=0x800d5bec;break;} // 7c64432e
case 0x800d5bec:{if(!word((r[13]+uint32_t(7136)),r[4]))return {};c.pc=0x800d5bf0;break;} // 808d1be0
case 0x800d5bf0:{if(!s.storeHalf||!s.storeHalf(s.user,r[4]+r[7],uint16_t(r[3])))return {};c.pc=0x800d5bf4;break;} // 7c643b2e
case 0x800d5bf4:{if(!word((r[13]+uint32_t(7136)),r[4]))return {};c.pc=0x800d5bf8;break;} // 808d1be0
case 0x800d5bf8:{if(!s.storeHalf||!s.storeHalf(s.user,r[4]+r[6],uint16_t(r[3])))return {};c.pc=0x800d5bfc;break;} // 7c64332e
case 0x800d5bfc:{if(!word((r[13]+uint32_t(7136)),r[4]))return {};c.pc=0x800d5c00;break;} // 808d1be0
case 0x800d5c00:{if(!s.storeHalf||!s.storeHalf(s.user,r[4]+r[5],uint16_t(r[3])))return {};c.pc=0x800d5c04;break;} // 7c642b2e
case 0x800d5c04:{if(!word((r[13]+uint32_t(7136)),r[4]))return {};c.pc=0x800d5c08;break;} // 808d1be0
case 0x800d5c08:{if(!s.storeHalf||!s.storeHalf(s.user,r[4]+r[0],uint16_t(r[3])))return {};c.pc=0x800d5c0c;break;} // 7c64032e
case 0x800d5c0c:{--c.ctr;c.pc=(c.ctr!=0)&&(true)?0x800d5b44:0x800d5c10;break;} // 4200ff38
case 0x800d5c10:{r[0]=uint32_t(25);c.pc=0x800d5c14;break;} // 38000019
case 0x800d5c14:{c.ctr=r[0];c.pc=0x800d5c18;break;} // 7c0903a6
case 0x800d5c18:{if(!word((r[13]+uint32_t(7136)),r[5]))return {};c.pc=0x800d5c1c;break;} // 80ad1be0
case 0x800d5c1c:{r[0]=r[3]+uint32_t(506);c.pc=0x800d5c20;break;} // 380301fa
case 0x800d5c20:{r[4]=uint32_t(0);c.pc=0x800d5c24;break;} // 38800000
case 0x800d5c24:{r[10]=r[3]+uint32_t(508);c.pc=0x800d5c28;break;} // 394301fc
case 0x800d5c28:{if(!s.storeHalf||!s.storeHalf(s.user,r[5]+r[0],uint16_t(r[4])))return {};c.pc=0x800d5c2c;break;} // 7c85032e
case 0x800d5c2c:{r[9]=r[3]+uint32_t(510);c.pc=0x800d5c30;break;} // 392301fe
case 0x800d5c30:{r[8]=r[3]+uint32_t(512);c.pc=0x800d5c34;break;} // 39030200
case 0x800d5c34:{r[7]=r[3]+uint32_t(514);c.pc=0x800d5c38;break;} // 38e30202
case 0x800d5c38:{if(!word((r[13]+uint32_t(7136)),r[11]))return {};c.pc=0x800d5c3c;break;} // 816d1be0
case 0x800d5c3c:{r[6]=r[3]+uint32_t(516);c.pc=0x800d5c40;break;} // 38c30204
case 0x800d5c40:{r[5]=r[3]+uint32_t(518);c.pc=0x800d5c44;break;} // 38a30206
case 0x800d5c44:{r[0]=r[3]+uint32_t(520);c.pc=0x800d5c48;break;} // 38030208
case 0x800d5c48:{if(!s.storeHalf||!s.storeHalf(s.user,r[11]+r[10],uint16_t(r[4])))return {};c.pc=0x800d5c4c;break;} // 7c8b532e
case 0x800d5c4c:{r[3]=r[3]+uint32_t(16);c.pc=0x800d5c50;break;} // 38630010
case 0x800d5c50:{if(!word((r[13]+uint32_t(7136)),r[10]))return {};c.pc=0x800d5c54;break;} // 814d1be0
case 0x800d5c54:{if(!s.storeHalf||!s.storeHalf(s.user,r[10]+r[9],uint16_t(r[4])))return {};c.pc=0x800d5c58;break;} // 7c8a4b2e
case 0x800d5c58:{if(!word((r[13]+uint32_t(7136)),r[9]))return {};c.pc=0x800d5c5c;break;} // 812d1be0
case 0x800d5c5c:{if(!s.storeHalf||!s.storeHalf(s.user,r[9]+r[8],uint16_t(r[4])))return {};c.pc=0x800d5c60;break;} // 7c89432e
case 0x800d5c60:{if(!word((r[13]+uint32_t(7136)),r[8]))return {};c.pc=0x800d5c64;break;} // 810d1be0
case 0x800d5c64:{if(!s.storeHalf||!s.storeHalf(s.user,r[8]+r[7],uint16_t(r[4])))return {};c.pc=0x800d5c68;break;} // 7c883b2e
case 0x800d5c68:{if(!word((r[13]+uint32_t(7136)),r[7]))return {};c.pc=0x800d5c6c;break;} // 80ed1be0
case 0x800d5c6c:{if(!s.storeHalf||!s.storeHalf(s.user,r[7]+r[6],uint16_t(r[4])))return {};c.pc=0x800d5c70;break;} // 7c87332e
case 0x800d5c70:{if(!word((r[13]+uint32_t(7136)),r[6]))return {};c.pc=0x800d5c74;break;} // 80cd1be0
case 0x800d5c74:{if(!s.storeHalf||!s.storeHalf(s.user,r[6]+r[5],uint16_t(r[4])))return {};c.pc=0x800d5c78;break;} // 7c862b2e
case 0x800d5c78:{if(!word((r[13]+uint32_t(7136)),r[5]))return {};c.pc=0x800d5c7c;break;} // 80ad1be0
case 0x800d5c7c:{if(!s.storeHalf||!s.storeHalf(s.user,r[5]+r[0],uint16_t(r[4])))return {};c.pc=0x800d5c80;break;} // 7c85032e
case 0x800d5c80:{--c.ctr;c.pc=(c.ctr!=0)&&(true)?0x800d5c18:0x800d5c84;break;} // 4200ff98
case 0x800d5c84:{r[0]=uint32_t(25);c.pc=0x800d5c88;break;} // 38000019
case 0x800d5c88:{c.ctr=r[0];c.pc=0x800d5c8c;break;} // 7c0903a6
case 0x800d5c8c:{if(!word((r[13]+uint32_t(7136)),r[5]))return {};c.pc=0x800d5c90;break;} // 80ad1be0
case 0x800d5c90:{r[3]=r[4]+uint32_t(906);c.pc=0x800d5c94;break;} // 3864038a
case 0x800d5c94:{r[0]=uint32_t(0);c.pc=0x800d5c98;break;} // 38000000
case 0x800d5c98:{r[10]=r[4]+uint32_t(908);c.pc=0x800d5c9c;break;} // 3944038c
case 0x800d5c9c:{if(!s.storeHalf||!s.storeHalf(s.user,r[5]+r[3],uint16_t(r[0])))return {};c.pc=0x800d5ca0;break;} // 7c051b2e
case 0x800d5ca0:{r[9]=r[4]+uint32_t(910);c.pc=0x800d5ca4;break;} // 3924038e
case 0x800d5ca4:{r[8]=r[4]+uint32_t(912);c.pc=0x800d5ca8;break;} // 39040390
case 0x800d5ca8:{r[7]=r[4]+uint32_t(914);c.pc=0x800d5cac;break;} // 38e40392
case 0x800d5cac:{if(!word((r[13]+uint32_t(7136)),r[11]))return {};c.pc=0x800d5cb0;break;} // 816d1be0
case 0x800d5cb0:{r[6]=r[4]+uint32_t(916);c.pc=0x800d5cb4;break;} // 38c40394
case 0x800d5cb4:{r[5]=r[4]+uint32_t(918);c.pc=0x800d5cb8;break;} // 38a40396
case 0x800d5cb8:{r[3]=r[4]+uint32_t(920);c.pc=0x800d5cbc;break;} // 38640398
case 0x800d5cbc:{if(!s.storeHalf||!s.storeHalf(s.user,r[11]+r[10],uint16_t(r[0])))return {};c.pc=0x800d5cc0;break;} // 7c0b532e
case 0x800d5cc0:{r[4]=r[4]+uint32_t(16);c.pc=0x800d5cc4;break;} // 38840010
case 0x800d5cc4:{if(!word((r[13]+uint32_t(7136)),r[10]))return {};c.pc=0x800d5cc8;break;} // 814d1be0
case 0x800d5cc8:{if(!s.storeHalf||!s.storeHalf(s.user,r[10]+r[9],uint16_t(r[0])))return {};c.pc=0x800d5ccc;break;} // 7c0a4b2e
case 0x800d5ccc:{if(!word((r[13]+uint32_t(7136)),r[9]))return {};c.pc=0x800d5cd0;break;} // 812d1be0
case 0x800d5cd0:{if(!s.storeHalf||!s.storeHalf(s.user,r[9]+r[8],uint16_t(r[0])))return {};c.pc=0x800d5cd4;break;} // 7c09432e
case 0x800d5cd4:{if(!word((r[13]+uint32_t(7136)),r[8]))return {};c.pc=0x800d5cd8;break;} // 810d1be0
case 0x800d5cd8:{if(!s.storeHalf||!s.storeHalf(s.user,r[8]+r[7],uint16_t(r[0])))return {};c.pc=0x800d5cdc;break;} // 7c083b2e
case 0x800d5cdc:{if(!word((r[13]+uint32_t(7136)),r[7]))return {};c.pc=0x800d5ce0;break;} // 80ed1be0
case 0x800d5ce0:{if(!s.storeHalf||!s.storeHalf(s.user,r[7]+r[6],uint16_t(r[0])))return {};c.pc=0x800d5ce4;break;} // 7c07332e
case 0x800d5ce4:{if(!word((r[13]+uint32_t(7136)),r[6]))return {};c.pc=0x800d5ce8;break;} // 80cd1be0
case 0x800d5ce8:{if(!s.storeHalf||!s.storeHalf(s.user,r[6]+r[5],uint16_t(r[0])))return {};c.pc=0x800d5cec;break;} // 7c062b2e
case 0x800d5cec:{if(!word((r[13]+uint32_t(7136)),r[5]))return {};c.pc=0x800d5cf0;break;} // 80ad1be0
case 0x800d5cf0:{if(!s.storeHalf||!s.storeHalf(s.user,r[5]+r[3],uint16_t(r[0])))return {};c.pc=0x800d5cf4;break;} // 7c051b2e
case 0x800d5cf4:{--c.ctr;c.pc=(c.ctr!=0)&&(true)?0x800d5c8c:0x800d5cf8;break;} // 4200ff98
case 0x800d5cf8:{if(!word((r[13]+uint32_t(7136)),r[3]))return {};c.pc=0x800d5cfc;break;} // 806d1be0
case 0x800d5cfc:{r[9]=uint32_t(10);c.pc=0x800d5d00;break;} // 3920000a
case 0x800d5d00:{r[8]=uint32_t(5);c.pc=0x800d5d04;break;} // 39000005
case 0x800d5d04:{r[7]=uint32_t(3);c.pc=0x800d5d08;break;} // 38e00003
case 0x800d5d08:{if(!s.storeHalf||!s.storeHalf(s.user,(r[3]+uint32_t(114)),uint16_t(r[9])))return {};c.pc=0x800d5d0c;break;} // b1230072
case 0x800d5d0c:{r[6]=uint32_t(1);c.pc=0x800d5d10;break;} // 38c00001
case 0x800d5d10:{if(!word((r[2]+uint32_t(-23492)),f[0]))return {};c.pc=0x800d5d14;break;} // c002a43c
case 0x800d5d14:{r[4]=uint32_t(2);c.pc=0x800d5d18;break;} // 38800002
case 0x800d5d18:{if(!word((r[13]+uint32_t(7136)),r[5]))return {};c.pc=0x800d5d1c;break;} // 80ad1be0
case 0x800d5d1c:{r[3]=r[0]|r[0];c.pc=0x800d5d20;break;} // 7c030378
case 0x800d5d20:{r[10]=r[0]|r[0];c.pc=0x800d5d24;break;} // 7c0a0378
case 0x800d5d24:{if(!s.storeHalf||!s.storeHalf(s.user,(r[5]+uint32_t(142)),uint16_t(r[9])))return {};c.pc=0x800d5d28;break;} // b125008e
case 0x800d5d28:{if(!word((r[13]+uint32_t(7136)),r[5]))return {};c.pc=0x800d5d2c;break;} // 80ad1be0
case 0x800d5d2c:{if(!s.storeHalf||!s.storeHalf(s.user,(r[5]+uint32_t(118)),uint16_t(r[8])))return {};c.pc=0x800d5d30;break;} // b1050076
case 0x800d5d30:{if(!word((r[13]+uint32_t(7136)),r[5]))return {};c.pc=0x800d5d34;break;} // 80ad1be0
case 0x800d5d34:{if(!s.storeHalf||!s.storeHalf(s.user,(r[5]+uint32_t(144)),uint16_t(r[8])))return {};c.pc=0x800d5d38;break;} // b1050090
case 0x800d5d38:{if(!word((r[13]+uint32_t(7136)),r[5]))return {};c.pc=0x800d5d3c;break;} // 80ad1be0
case 0x800d5d3c:{if(!s.storeHalf||!s.storeHalf(s.user,(r[5]+uint32_t(146)),uint16_t(r[7])))return {};c.pc=0x800d5d40;break;} // b0e50092
case 0x800d5d40:{if(!word((r[13]+uint32_t(7136)),r[5]))return {};c.pc=0x800d5d44;break;} // 80ad1be0
case 0x800d5d44:{if(!s.storeHalf||!s.storeHalf(s.user,(r[5]+uint32_t(148)),uint16_t(r[7])))return {};c.pc=0x800d5d48;break;} // b0e50094
case 0x800d5d48:{if(!word((r[13]+uint32_t(7136)),r[7]))return {};c.pc=0x800d5d4c;break;} // 80ed1be0
case 0x800d5d4c:{uint32_t value;if(!half((r[7]+uint32_t(114)),value))return {};r[5]=signHalf(value);c.pc=0x800d5d50;break;} // a8a70072
case 0x800d5d50:{if(!s.storeHalf||!s.storeHalf(s.user,(r[7]+uint32_t(112)),uint16_t(r[5])))return {};c.pc=0x800d5d54;break;} // b0a70070
case 0x800d5d54:{if(!word((r[13]+uint32_t(7136)),r[7]))return {};c.pc=0x800d5d58;break;} // 80ed1be0
case 0x800d5d58:{uint32_t value;if(!half((r[7]+uint32_t(118)),value))return {};r[5]=signHalf(value);c.pc=0x800d5d5c;break;} // a8a70076
case 0x800d5d5c:{if(!s.storeHalf||!s.storeHalf(s.user,(r[7]+uint32_t(116)),uint16_t(r[5])))return {};c.pc=0x800d5d60;break;} // b0a70074
case 0x800d5d60:{if(!word((r[13]+uint32_t(7136)),r[5]))return {};c.pc=0x800d5d64;break;} // 80ad1be0
case 0x800d5d64:{if(!s.storeHalf||!s.storeHalf(s.user,(r[5]+uint32_t(122)),uint16_t(r[0])))return {};c.pc=0x800d5d68;break;} // b005007a
case 0x800d5d68:{if(!word((r[13]+uint32_t(7136)),r[5]))return {};c.pc=0x800d5d6c;break;} // 80ad1be0
case 0x800d5d6c:{if(!s.storeHalf||!s.storeHalf(s.user,(r[5]+uint32_t(126)),uint16_t(r[0])))return {};c.pc=0x800d5d70;break;} // b005007e
case 0x800d5d70:{if(!word((r[13]+uint32_t(7136)),r[5]))return {};c.pc=0x800d5d74;break;} // 80ad1be0
case 0x800d5d74:{if(!s.storeHalf||!s.storeHalf(s.user,(r[5]+uint32_t(124)),uint16_t(r[0])))return {};c.pc=0x800d5d78;break;} // b005007c
case 0x800d5d78:{if(!word((r[13]+uint32_t(7136)),r[5]))return {};c.pc=0x800d5d7c;break;} // 80ad1be0
case 0x800d5d7c:{if(!s.storeHalf||!s.storeHalf(s.user,(r[5]+uint32_t(128)),uint16_t(r[0])))return {};c.pc=0x800d5d80;break;} // b0050080
case 0x800d5d80:{if(!word((r[13]+uint32_t(7136)),r[5]))return {};c.pc=0x800d5d84;break;} // 80ad1be0
case 0x800d5d84:{if(!store((r[5]+uint32_t(132)),f[0]))return {};c.pc=0x800d5d88;break;} // d0050084
case 0x800d5d88:{if(!word((r[13]+uint32_t(7136)),r[5]))return {};c.pc=0x800d5d8c;break;} // 80ad1be0
case 0x800d5d8c:{if(!s.storeHalf||!s.storeHalf(s.user,(r[5]+uint32_t(136)),uint16_t(r[0])))return {};c.pc=0x800d5d90;break;} // b0050088
case 0x800d5d90:{if(!word((r[13]+uint32_t(7136)),r[5]))return {};c.pc=0x800d5d94;break;} // 80ad1be0
case 0x800d5d94:{if(!s.storeHalf||!s.storeHalf(s.user,(r[5]+uint32_t(138)),uint16_t(r[6])))return {};c.pc=0x800d5d98;break;} // b0c5008a
case 0x800d5d98:{if(!word((r[13]+uint32_t(7136)),r[5]))return {};c.pc=0x800d5d9c;break;} // 80ad1be0
case 0x800d5d9c:{if(!s.storeByte||!s.storeByte(s.user,(r[5]+uint32_t(152)),uint8_t(r[0])))return {};c.pc=0x800d5da0;break;} // 98050098
case 0x800d5da0:{if(!word((r[13]+uint32_t(7136)),r[5]))return {};c.pc=0x800d5da4;break;} // 80ad1be0
case 0x800d5da4:{if(!s.storeByte||!s.storeByte(s.user,(r[5]+uint32_t(153)),uint8_t(r[0])))return {};c.pc=0x800d5da8;break;} // 98050099
case 0x800d5da8:{c.ctr=r[4];c.pc=0x800d5dac;break;} // 7c8903a6
case 0x800d5dac:{if(!word((r[13]+uint32_t(7136)),r[4]))return {};c.pc=0x800d5db0;break;} // 808d1be0
case 0x800d5db0:{compare(c,0,signedWord(r[3]),0);c.pc=0x800d5db4;break;} // 2c030000
case 0x800d5db4:{r[5]=r[4]+r[10];c.pc=0x800d5db8;break;} // 7ca45214
case 0x800d5db8:{c.pc=(true)&&((c.condition[2]==true))?0x800d5ddc:0x800d5dbc;break;} // 41820024
case 0x800d5dbc:{if(!s.storeHalf||!s.storeHalf(s.user,(r[5]+uint32_t(0)),uint16_t(r[0])))return {};c.pc=0x800d5dc0;break;} // b0050000
case 0x800d5dc0:{if(!s.storeHalf||!s.storeHalf(s.user,(r[5]+uint32_t(4)),uint16_t(r[9])))return {};c.pc=0x800d5dc4;break;} // b1250004
case 0x800d5dc4:{uint32_t value;if(!half((r[5]+uint32_t(4)),value))return {};r[4]=signHalf(value);c.pc=0x800d5dc8;break;} // a8850004
case 0x800d5dc8:{if(!s.storeHalf||!s.storeHalf(s.user,(r[5]+uint32_t(2)),uint16_t(r[4])))return {};c.pc=0x800d5dcc;break;} // b0850002
case 0x800d5dcc:{uint32_t value;if(!half((r[5]+uint32_t(4)),value))return {};r[4]=signHalf(value);c.pc=0x800d5dd0;break;} // a8850004
case 0x800d5dd0:{if(!s.storeHalf||!s.storeHalf(s.user,(r[5]+uint32_t(6)),uint16_t(r[4])))return {};c.pc=0x800d5dd4;break;} // b0850006
case 0x800d5dd4:{if(!s.storeHalf||!s.storeHalf(s.user,(r[5]+uint32_t(10)),uint16_t(r[0])))return {};c.pc=0x800d5dd8;break;} // b005000a
case 0x800d5dd8:{if(!s.storeHalf||!s.storeHalf(s.user,(r[5]+uint32_t(12)),uint16_t(r[0])))return {};c.pc=0x800d5ddc;break;} // b005000c
case 0x800d5ddc:{if(!word((r[13]+uint32_t(7136)),r[4]))return {};c.pc=0x800d5de0;break;} // 808d1be0
case 0x800d5de0:{const uint64_t value=uint64_t(r[3])+uint32_t(1);c.carry=value>>32;r[3]=uint32_t(value);compare(c,0,signedWord(r[3]),0);c.pc=0x800d5de4;break;} // 34630001
case 0x800d5de4:{r[10]=r[10]+uint32_t(14);c.pc=0x800d5de8;break;} // 394a000e
case 0x800d5de8:{r[5]=r[4]+r[10];c.pc=0x800d5dec;break;} // 7ca45214
case 0x800d5dec:{c.pc=(true)&&((c.condition[2]==true))?0x800d5e10:0x800d5df0;break;} // 41820024
case 0x800d5df0:{if(!s.storeHalf||!s.storeHalf(s.user,(r[5]+uint32_t(0)),uint16_t(r[0])))return {};c.pc=0x800d5df4;break;} // b0050000
case 0x800d5df4:{if(!s.storeHalf||!s.storeHalf(s.user,(r[5]+uint32_t(4)),uint16_t(r[9])))return {};c.pc=0x800d5df8;break;} // b1250004
case 0x800d5df8:{uint32_t value;if(!half((r[5]+uint32_t(4)),value))return {};r[4]=signHalf(value);c.pc=0x800d5dfc;break;} // a8850004
case 0x800d5dfc:{if(!s.storeHalf||!s.storeHalf(s.user,(r[5]+uint32_t(2)),uint16_t(r[4])))return {};c.pc=0x800d5e00;break;} // b0850002
case 0x800d5e00:{uint32_t value;if(!half((r[5]+uint32_t(4)),value))return {};r[4]=signHalf(value);c.pc=0x800d5e04;break;} // a8850004
case 0x800d5e04:{if(!s.storeHalf||!s.storeHalf(s.user,(r[5]+uint32_t(6)),uint16_t(r[4])))return {};c.pc=0x800d5e08;break;} // b0850006
case 0x800d5e08:{if(!s.storeHalf||!s.storeHalf(s.user,(r[5]+uint32_t(10)),uint16_t(r[0])))return {};c.pc=0x800d5e0c;break;} // b005000a
case 0x800d5e0c:{if(!s.storeHalf||!s.storeHalf(s.user,(r[5]+uint32_t(12)),uint16_t(r[0])))return {};c.pc=0x800d5e10;break;} // b005000c
case 0x800d5e10:{if(!word((r[13]+uint32_t(7136)),r[4]))return {};c.pc=0x800d5e14;break;} // 808d1be0
case 0x800d5e14:{const uint64_t value=uint64_t(r[3])+uint32_t(1);c.carry=value>>32;r[3]=uint32_t(value);compare(c,0,signedWord(r[3]),0);c.pc=0x800d5e18;break;} // 34630001
case 0x800d5e18:{r[10]=r[10]+uint32_t(14);c.pc=0x800d5e1c;break;} // 394a000e
case 0x800d5e1c:{r[5]=r[4]+r[10];c.pc=0x800d5e20;break;} // 7ca45214
case 0x800d5e20:{c.pc=(true)&&((c.condition[2]==true))?0x800d5e44:0x800d5e24;break;} // 41820024
case 0x800d5e24:{if(!s.storeHalf||!s.storeHalf(s.user,(r[5]+uint32_t(0)),uint16_t(r[0])))return {};c.pc=0x800d5e28;break;} // b0050000
case 0x800d5e28:{if(!s.storeHalf||!s.storeHalf(s.user,(r[5]+uint32_t(4)),uint16_t(r[9])))return {};c.pc=0x800d5e2c;break;} // b1250004
case 0x800d5e2c:{uint32_t value;if(!half((r[5]+uint32_t(4)),value))return {};r[4]=signHalf(value);c.pc=0x800d5e30;break;} // a8850004
case 0x800d5e30:{if(!s.storeHalf||!s.storeHalf(s.user,(r[5]+uint32_t(2)),uint16_t(r[4])))return {};c.pc=0x800d5e34;break;} // b0850002
case 0x800d5e34:{uint32_t value;if(!half((r[5]+uint32_t(4)),value))return {};r[4]=signHalf(value);c.pc=0x800d5e38;break;} // a8850004
case 0x800d5e38:{if(!s.storeHalf||!s.storeHalf(s.user,(r[5]+uint32_t(6)),uint16_t(r[4])))return {};c.pc=0x800d5e3c;break;} // b0850006
case 0x800d5e3c:{if(!s.storeHalf||!s.storeHalf(s.user,(r[5]+uint32_t(10)),uint16_t(r[0])))return {};c.pc=0x800d5e40;break;} // b005000a
case 0x800d5e40:{if(!s.storeHalf||!s.storeHalf(s.user,(r[5]+uint32_t(12)),uint16_t(r[0])))return {};c.pc=0x800d5e44;break;} // b005000c
case 0x800d5e44:{if(!word((r[13]+uint32_t(7136)),r[4]))return {};c.pc=0x800d5e48;break;} // 808d1be0
case 0x800d5e48:{const uint64_t value=uint64_t(r[3])+uint32_t(1);c.carry=value>>32;r[3]=uint32_t(value);compare(c,0,signedWord(r[3]),0);c.pc=0x800d5e4c;break;} // 34630001
case 0x800d5e4c:{r[10]=r[10]+uint32_t(14);c.pc=0x800d5e50;break;} // 394a000e
case 0x800d5e50:{r[5]=r[4]+r[10];c.pc=0x800d5e54;break;} // 7ca45214
case 0x800d5e54:{c.pc=(true)&&((c.condition[2]==true))?0x800d5e78:0x800d5e58;break;} // 41820024
case 0x800d5e58:{if(!s.storeHalf||!s.storeHalf(s.user,(r[5]+uint32_t(0)),uint16_t(r[0])))return {};c.pc=0x800d5e5c;break;} // b0050000
case 0x800d5e5c:{if(!s.storeHalf||!s.storeHalf(s.user,(r[5]+uint32_t(4)),uint16_t(r[9])))return {};c.pc=0x800d5e60;break;} // b1250004
case 0x800d5e60:{uint32_t value;if(!half((r[5]+uint32_t(4)),value))return {};r[4]=signHalf(value);c.pc=0x800d5e64;break;} // a8850004
case 0x800d5e64:{if(!s.storeHalf||!s.storeHalf(s.user,(r[5]+uint32_t(2)),uint16_t(r[4])))return {};c.pc=0x800d5e68;break;} // b0850002
case 0x800d5e68:{uint32_t value;if(!half((r[5]+uint32_t(4)),value))return {};r[4]=signHalf(value);c.pc=0x800d5e6c;break;} // a8850004
case 0x800d5e6c:{if(!s.storeHalf||!s.storeHalf(s.user,(r[5]+uint32_t(6)),uint16_t(r[4])))return {};c.pc=0x800d5e70;break;} // b0850006
case 0x800d5e70:{if(!s.storeHalf||!s.storeHalf(s.user,(r[5]+uint32_t(10)),uint16_t(r[0])))return {};c.pc=0x800d5e74;break;} // b005000a
case 0x800d5e74:{if(!s.storeHalf||!s.storeHalf(s.user,(r[5]+uint32_t(12)),uint16_t(r[0])))return {};c.pc=0x800d5e78;break;} // b005000c
case 0x800d5e78:{r[10]=r[10]+uint32_t(14);c.pc=0x800d5e7c;break;} // 394a000e
case 0x800d5e7c:{r[3]=r[3]+uint32_t(1);c.pc=0x800d5e80;break;} // 38630001
case 0x800d5e80:{--c.ctr;c.pc=(c.ctr!=0)&&(true)?0x800d5dac:0x800d5e84;break;} // 4200ff2c
case 0x800d5e84:{r[3]=uint32_t(-2144534528);c.pc=0x800d5e88;break;} // 3c60802d
case 0x800d5e88:{r[3]=r[3]+uint32_t(-14552);c.pc=0x800d5e8c;break;} // 3863c728
case 0x800d5e8c:{if(!external(0x80081b8c,0x800d5e90))return {};break;} // 4bfabd01
case 0x800d5e90:{if(!word((r[13]+uint32_t(7136)),r[5]))return {};c.pc=0x800d5e94;break;} // 80ad1be0
case 0x800d5e94:{r[4]=r[3]|r[3];c.pc=0x800d5e98;break;} // 7c641b78
case 0x800d5e98:{r[3]=r[5]+uint32_t(1472);c.pc=0x800d5e9c;break;} // 386505c0
case 0x800d5e9c:{if(!external(0x8026d5fc,0x800d5ea0))return {};break;} // 48197761
case 0x800d5ea0:{if(!word((r[1]+uint32_t(20)),r[0]))return {};c.pc=0x800d5ea4;break;} // 80010014
case 0x800d5ea4:{c.lr=r[0];c.pc=0x800d5ea8;break;} // 7c0803a6
case 0x800d5ea8:{r[1]=r[1]+uint32_t(16);c.pc=0x800d5eac;break;} // 38210010
case 0x800d5eac:{c.pc=c.lr;break;} // 4e800020
// pouchCheckItem 800d4ed4/1f8 SHA256 42deab825c1637d84ab4ad27431a9571fbb92f349716b88a6eba7b736779ba57
case 0x800d4ed4:{compare(c,0,signedWord(r[3]),121);c.pc=0x800d4ed8;break;} // 2c030079
case 0x800d4ed8:{c.pc=(true)&&((c.condition[0]==true))?0x800d4fac:0x800d4edc;break;} // 418000d4
case 0x800d4edc:{compare(c,0,signedWord(r[3]),236);c.pc=0x800d4ee0;break;} // 2c0300ec
case 0x800d4ee0:{c.pc=(true)&&((c.condition[0]==false))?0x800d4fac:0x800d4ee4;break;} // 408000cc
case 0x800d4ee4:{r[0]=uint32_t(2);c.pc=0x800d4ee8;break;} // 38000002
case 0x800d4ee8:{if(!word((r[13]+uint32_t(7136)),r[4]))return {};c.pc=0x800d4eec;break;} // 808d1be0
case 0x800d4eec:{r[6]=uint32_t(0);c.pc=0x800d4ef0;break;} // 38c00000
case 0x800d4ef0:{r[5]=uint32_t(0);c.pc=0x800d4ef4;break;} // 38a00000
case 0x800d4ef4:{c.ctr=r[0];c.pc=0x800d4ef8;break;} // 7c0903a6
case 0x800d4ef8:{uint32_t value;if(!half((r[4]+uint32_t(402)),value))return {};r[0]=signHalf(value);c.pc=0x800d4efc;break;} // a8040192
case 0x800d4efc:{compare(c,0,signedWord(r[3]),signedWord(r[0]));c.pc=0x800d4f00;break;} // 7c030000
case 0x800d4f00:{c.pc=(true)&&((c.condition[2]==false))?0x800d4f08:0x800d4f04;break;} // 40820008
case 0x800d4f04:{r[6]=r[6]+uint32_t(1);c.pc=0x800d4f08;break;} // 38c60001
case 0x800d4f08:{uint32_t value;if(!half((r[4]+uint32_t(404)),value))return {};r[0]=signHalf(value);c.pc=0x800d4f0c;break;} // a8040194
case 0x800d4f0c:{compare(c,0,signedWord(r[3]),signedWord(r[0]));c.pc=0x800d4f10;break;} // 7c030000
case 0x800d4f10:{c.pc=(true)&&((c.condition[2]==false))?0x800d4f18:0x800d4f14;break;} // 40820008
case 0x800d4f14:{r[6]=r[6]+uint32_t(1);c.pc=0x800d4f18;break;} // 38c60001
case 0x800d4f18:{uint32_t value;if(!half((r[4]+uint32_t(406)),value))return {};r[0]=signHalf(value);c.pc=0x800d4f1c;break;} // a8040196
case 0x800d4f1c:{compare(c,0,signedWord(r[3]),signedWord(r[0]));c.pc=0x800d4f20;break;} // 7c030000
case 0x800d4f20:{c.pc=(true)&&((c.condition[2]==false))?0x800d4f28:0x800d4f24;break;} // 40820008
case 0x800d4f24:{r[6]=r[6]+uint32_t(1);c.pc=0x800d4f28;break;} // 38c60001
case 0x800d4f28:{uint32_t value;if(!half((r[4]+uint32_t(408)),value))return {};r[0]=signHalf(value);c.pc=0x800d4f2c;break;} // a8040198
case 0x800d4f2c:{compare(c,0,signedWord(r[3]),signedWord(r[0]));c.pc=0x800d4f30;break;} // 7c030000
case 0x800d4f30:{c.pc=(true)&&((c.condition[2]==false))?0x800d4f38:0x800d4f34;break;} // 40820008
case 0x800d4f34:{r[6]=r[6]+uint32_t(1);c.pc=0x800d4f38;break;} // 38c60001
case 0x800d4f38:{uint32_t value;if(!half((r[4]+uint32_t(410)),value))return {};r[0]=signHalf(value);c.pc=0x800d4f3c;break;} // a804019a
case 0x800d4f3c:{compare(c,0,signedWord(r[3]),signedWord(r[0]));c.pc=0x800d4f40;break;} // 7c030000
case 0x800d4f40:{c.pc=(true)&&((c.condition[2]==false))?0x800d4f48:0x800d4f44;break;} // 40820008
case 0x800d4f44:{r[6]=r[6]+uint32_t(1);c.pc=0x800d4f48;break;} // 38c60001
case 0x800d4f48:{uint32_t value;if(!half((r[4]+uint32_t(412)),value))return {};r[0]=signHalf(value);c.pc=0x800d4f4c;break;} // a804019c
case 0x800d4f4c:{compare(c,0,signedWord(r[3]),signedWord(r[0]));c.pc=0x800d4f50;break;} // 7c030000
case 0x800d4f50:{c.pc=(true)&&((c.condition[2]==false))?0x800d4f58:0x800d4f54;break;} // 40820008
case 0x800d4f54:{r[6]=r[6]+uint32_t(1);c.pc=0x800d4f58;break;} // 38c60001
case 0x800d4f58:{uint32_t value;if(!half((r[4]+uint32_t(414)),value))return {};r[0]=signHalf(value);c.pc=0x800d4f5c;break;} // a804019e
case 0x800d4f5c:{compare(c,0,signedWord(r[3]),signedWord(r[0]));c.pc=0x800d4f60;break;} // 7c030000
case 0x800d4f60:{c.pc=(true)&&((c.condition[2]==false))?0x800d4f68:0x800d4f64;break;} // 40820008
case 0x800d4f64:{r[6]=r[6]+uint32_t(1);c.pc=0x800d4f68;break;} // 38c60001
case 0x800d4f68:{uint32_t value;if(!half((r[4]+uint32_t(416)),value))return {};r[0]=signHalf(value);c.pc=0x800d4f6c;break;} // a80401a0
case 0x800d4f6c:{compare(c,0,signedWord(r[3]),signedWord(r[0]));c.pc=0x800d4f70;break;} // 7c030000
case 0x800d4f70:{c.pc=(true)&&((c.condition[2]==false))?0x800d4f78:0x800d4f74;break;} // 40820008
case 0x800d4f74:{r[6]=r[6]+uint32_t(1);c.pc=0x800d4f78;break;} // 38c60001
case 0x800d4f78:{uint32_t value;if(!half((r[4]+uint32_t(418)),value))return {};r[0]=signHalf(value);c.pc=0x800d4f7c;break;} // a80401a2
case 0x800d4f7c:{compare(c,0,signedWord(r[3]),signedWord(r[0]));c.pc=0x800d4f80;break;} // 7c030000
case 0x800d4f80:{c.pc=(true)&&((c.condition[2]==false))?0x800d4f88:0x800d4f84;break;} // 40820008
case 0x800d4f84:{r[6]=r[6]+uint32_t(1);c.pc=0x800d4f88;break;} // 38c60001
case 0x800d4f88:{uint32_t value;if(!half((r[4]+uint32_t(420)),value))return {};r[0]=signHalf(value);c.pc=0x800d4f8c;break;} // a80401a4
case 0x800d4f8c:{compare(c,0,signedWord(r[3]),signedWord(r[0]));c.pc=0x800d4f90;break;} // 7c030000
case 0x800d4f90:{c.pc=(true)&&((c.condition[2]==false))?0x800d4f98:0x800d4f94;break;} // 40820008
case 0x800d4f94:{r[6]=r[6]+uint32_t(1);c.pc=0x800d4f98;break;} // 38c60001
case 0x800d4f98:{r[4]=r[4]+uint32_t(20);c.pc=0x800d4f9c;break;} // 38840014
case 0x800d4f9c:{r[5]=r[5]+uint32_t(9);c.pc=0x800d4fa0;break;} // 38a50009
case 0x800d4fa0:{--c.ctr;c.pc=(c.ctr!=0)&&(true)?0x800d4ef8:0x800d4fa4;break;} // 4200ff58
case 0x800d4fa4:{r[3]=r[6]|r[6];c.pc=0x800d4fa8;break;} // 7cc33378
case 0x800d4fa8:{c.pc=c.lr;break;} // 4e800020
case 0x800d4fac:{compare(c,0,signedWord(r[3]),1);c.pc=0x800d4fb0;break;} // 2c030001
case 0x800d4fb0:{c.pc=(true)&&((c.condition[0]==true))?0x800d4fec:0x800d4fb4;break;} // 4180003c
case 0x800d4fb4:{compare(c,0,signedWord(r[3]),121);c.pc=0x800d4fb8;break;} // 2c030079
case 0x800d4fb8:{c.pc=(true)&&((c.condition[0]==false))?0x800d4fec:0x800d4fbc;break;} // 40800034
case 0x800d4fbc:{r[0]=uint32_t(121);c.pc=0x800d4fc0;break;} // 38000079
case 0x800d4fc0:{if(!word((r[13]+uint32_t(7136)),r[4]))return {};c.pc=0x800d4fc4;break;} // 808d1be0
case 0x800d4fc4:{r[5]=uint32_t(0);c.pc=0x800d4fc8;break;} // 38a00000
case 0x800d4fc8:{c.ctr=r[0];c.pc=0x800d4fcc;break;} // 7c0903a6
case 0x800d4fcc:{uint32_t value;if(!half((r[4]+uint32_t(160)),value))return {};r[0]=signHalf(value);c.pc=0x800d4fd0;break;} // a80400a0
case 0x800d4fd0:{compare(c,0,signedWord(r[3]),signedWord(r[0]));c.pc=0x800d4fd4;break;} // 7c030000
case 0x800d4fd4:{c.pc=(true)&&((c.condition[2]==false))?0x800d4fdc:0x800d4fd8;break;} // 40820008
case 0x800d4fd8:{r[5]=r[5]+uint32_t(1);c.pc=0x800d4fdc;break;} // 38a50001
case 0x800d4fdc:{r[4]=r[4]+uint32_t(2);c.pc=0x800d4fe0;break;} // 38840002
case 0x800d4fe0:{--c.ctr;c.pc=(c.ctr!=0)&&(true)?0x800d4fcc:0x800d4fe4;break;} // 4200ffec
case 0x800d4fe4:{r[3]=r[5]|r[5];c.pc=0x800d4fe8;break;} // 7ca32b78
case 0x800d4fe8:{c.pc=c.lr;break;} // 4e800020
case 0x800d4fec:{compare(c,0,signedWord(r[3]),240);c.pc=0x800d4ff0;break;} // 2c0300f0
case 0x800d4ff0:{c.pc=(true)&&((c.condition[0]==true))?0x800d50c4:0x800d4ff4;break;} // 418000d4
case 0x800d4ff4:{compare(c,0,signedWord(r[3]),339);c.pc=0x800d4ff8;break;} // 2c030153
case 0x800d4ff8:{c.pc=(true)&&((c.condition[0]==false))?0x800d50c4:0x800d4ffc;break;} // 408000cc
case 0x800d4ffc:{r[5]=uint32_t(0);c.pc=0x800d5000;break;} // 38a00000
case 0x800d5000:{r[0]=uint32_t(20);c.pc=0x800d5004;break;} // 38000014
case 0x800d5004:{r[6]=r[5]|r[5];c.pc=0x800d5008;break;} // 7ca62b78
case 0x800d5008:{if(!word((r[13]+uint32_t(7136)),r[4]))return {};c.pc=0x800d500c;break;} // 808d1be0
case 0x800d500c:{c.ctr=r[0];c.pc=0x800d5010;break;} // 7c0903a6
case 0x800d5010:{uint32_t value;if(!half((r[4]+uint32_t(506)),value))return {};r[0]=signHalf(value);c.pc=0x800d5014;break;} // a80401fa
case 0x800d5014:{compare(c,0,signedWord(r[3]),signedWord(r[0]));c.pc=0x800d5018;break;} // 7c030000
case 0x800d5018:{c.pc=(true)&&((c.condition[2]==false))?0x800d5020:0x800d501c;break;} // 40820008
case 0x800d501c:{r[5]=r[5]+uint32_t(1);c.pc=0x800d5020;break;} // 38a50001
case 0x800d5020:{uint32_t value;if(!half((r[4]+uint32_t(508)),value))return {};r[0]=signHalf(value);c.pc=0x800d5024;break;} // a80401fc
case 0x800d5024:{compare(c,0,signedWord(r[3]),signedWord(r[0]));c.pc=0x800d5028;break;} // 7c030000
case 0x800d5028:{c.pc=(true)&&((c.condition[2]==false))?0x800d5030:0x800d502c;break;} // 40820008
case 0x800d502c:{r[5]=r[5]+uint32_t(1);c.pc=0x800d5030;break;} // 38a50001
case 0x800d5030:{uint32_t value;if(!half((r[4]+uint32_t(510)),value))return {};r[0]=signHalf(value);c.pc=0x800d5034;break;} // a80401fe
case 0x800d5034:{compare(c,0,signedWord(r[3]),signedWord(r[0]));c.pc=0x800d5038;break;} // 7c030000
case 0x800d5038:{c.pc=(true)&&((c.condition[2]==false))?0x800d5040:0x800d503c;break;} // 40820008
case 0x800d503c:{r[5]=r[5]+uint32_t(1);c.pc=0x800d5040;break;} // 38a50001
case 0x800d5040:{uint32_t value;if(!half((r[4]+uint32_t(512)),value))return {};r[0]=signHalf(value);c.pc=0x800d5044;break;} // a8040200
case 0x800d5044:{compare(c,0,signedWord(r[3]),signedWord(r[0]));c.pc=0x800d5048;break;} // 7c030000
case 0x800d5048:{c.pc=(true)&&((c.condition[2]==false))?0x800d5050:0x800d504c;break;} // 40820008
case 0x800d504c:{r[5]=r[5]+uint32_t(1);c.pc=0x800d5050;break;} // 38a50001
case 0x800d5050:{uint32_t value;if(!half((r[4]+uint32_t(514)),value))return {};r[0]=signHalf(value);c.pc=0x800d5054;break;} // a8040202
case 0x800d5054:{compare(c,0,signedWord(r[3]),signedWord(r[0]));c.pc=0x800d5058;break;} // 7c030000
case 0x800d5058:{c.pc=(true)&&((c.condition[2]==false))?0x800d5060:0x800d505c;break;} // 40820008
case 0x800d505c:{r[5]=r[5]+uint32_t(1);c.pc=0x800d5060;break;} // 38a50001
case 0x800d5060:{uint32_t value;if(!half((r[4]+uint32_t(516)),value))return {};r[0]=signHalf(value);c.pc=0x800d5064;break;} // a8040204
case 0x800d5064:{compare(c,0,signedWord(r[3]),signedWord(r[0]));c.pc=0x800d5068;break;} // 7c030000
case 0x800d5068:{c.pc=(true)&&((c.condition[2]==false))?0x800d5070:0x800d506c;break;} // 40820008
case 0x800d506c:{r[5]=r[5]+uint32_t(1);c.pc=0x800d5070;break;} // 38a50001
case 0x800d5070:{uint32_t value;if(!half((r[4]+uint32_t(518)),value))return {};r[0]=signHalf(value);c.pc=0x800d5074;break;} // a8040206
case 0x800d5074:{compare(c,0,signedWord(r[3]),signedWord(r[0]));c.pc=0x800d5078;break;} // 7c030000
case 0x800d5078:{c.pc=(true)&&((c.condition[2]==false))?0x800d5080:0x800d507c;break;} // 40820008
case 0x800d507c:{r[5]=r[5]+uint32_t(1);c.pc=0x800d5080;break;} // 38a50001
case 0x800d5080:{uint32_t value;if(!half((r[4]+uint32_t(520)),value))return {};r[0]=signHalf(value);c.pc=0x800d5084;break;} // a8040208
case 0x800d5084:{compare(c,0,signedWord(r[3]),signedWord(r[0]));c.pc=0x800d5088;break;} // 7c030000
case 0x800d5088:{c.pc=(true)&&((c.condition[2]==false))?0x800d5090:0x800d508c;break;} // 40820008
case 0x800d508c:{r[5]=r[5]+uint32_t(1);c.pc=0x800d5090;break;} // 38a50001
case 0x800d5090:{uint32_t value;if(!half((r[4]+uint32_t(522)),value))return {};r[0]=signHalf(value);c.pc=0x800d5094;break;} // a804020a
case 0x800d5094:{compare(c,0,signedWord(r[3]),signedWord(r[0]));c.pc=0x800d5098;break;} // 7c030000
case 0x800d5098:{c.pc=(true)&&((c.condition[2]==false))?0x800d50a0:0x800d509c;break;} // 40820008
case 0x800d509c:{r[5]=r[5]+uint32_t(1);c.pc=0x800d50a0;break;} // 38a50001
case 0x800d50a0:{uint32_t value;if(!half((r[4]+uint32_t(524)),value))return {};r[0]=signHalf(value);c.pc=0x800d50a4;break;} // a804020c
case 0x800d50a4:{compare(c,0,signedWord(r[3]),signedWord(r[0]));c.pc=0x800d50a8;break;} // 7c030000
case 0x800d50a8:{c.pc=(true)&&((c.condition[2]==false))?0x800d50b0:0x800d50ac;break;} // 40820008
case 0x800d50ac:{r[5]=r[5]+uint32_t(1);c.pc=0x800d50b0;break;} // 38a50001
case 0x800d50b0:{r[4]=r[4]+uint32_t(20);c.pc=0x800d50b4;break;} // 38840014
case 0x800d50b4:{r[6]=r[6]+uint32_t(9);c.pc=0x800d50b8;break;} // 38c60009
case 0x800d50b8:{--c.ctr;c.pc=(c.ctr!=0)&&(true)?0x800d5010:0x800d50bc;break;} // 4200ff58
case 0x800d50bc:{r[3]=r[5]|r[5];c.pc=0x800d50c0;break;} // 7ca32b78
case 0x800d50c0:{c.pc=c.lr;break;} // 4e800020
case 0x800d50c4:{r[3]=uint32_t(0);c.pc=0x800d50c8;break;} // 38600000
case 0x800d50c8:{c.pc=c.lr;break;} // 4e800020
// pouchGetItem 800d50cc/4a4 SHA256 7abf7872b6aedf92f82286b20aebcbad5b613f1f83616e7ee8cb661d7e47998c
case 0x800d50cc:{const auto target=(r[1]+uint32_t(-32));if(!store(target,r[1]))return {};r[1]=target;c.pc=0x800d50d0;break;} // 9421ffe0
case 0x800d50d0:{r[0]=c.lr;c.pc=0x800d50d4;break;} // 7c0802a6
case 0x800d50d4:{if(!store((r[1]+uint32_t(36)),r[0]))return {};c.pc=0x800d50d8;break;} // 90010024
case 0x800d50d8:{if(!store((r[1]+uint32_t(28)),r[31]))return {};c.pc=0x800d50dc;break;} // 93e1001c
case 0x800d50dc:{r[31]=r[3]|r[3];c.pc=0x800d50e0;break;} // 7c7f1b78
case 0x800d50e0:{compare(c,0,signedWord(r[31]),121);c.pc=0x800d50e4;break;} // 2c1f0079
case 0x800d50e4:{if(!store((r[1]+uint32_t(24)),r[30]))return {};c.pc=0x800d50e8;break;} // 93c10018
case 0x800d50e8:{if(!store((r[1]+uint32_t(20)),r[29]))return {};c.pc=0x800d50ec;break;} // 93a10014
case 0x800d50ec:{c.pc=(true)&&((c.condition[2]==false))?0x800d5110:0x800d50f0;break;} // 40820024
case 0x800d50f0:{if(!word((r[13]+uint32_t(7136)),r[4]))return {};c.pc=0x800d50f4;break;} // 808d1be0
case 0x800d50f4:{uint32_t value;if(!half((r[4]+uint32_t(120)),value))return {};r[3]=signHalf(value);c.pc=0x800d50f8;break;} // a8640078
case 0x800d50f8:{compare(c,0,signedWord(r[3]),999);c.pc=0x800d50fc;break;} // 2c0303e7
case 0x800d50fc:{c.pc=(true)&&((c.condition[0]==false))?0x800d5108:0x800d5100;break;} // 4080000c
case 0x800d5100:{r[0]=r[3]+uint32_t(1);c.pc=0x800d5104;break;} // 38030001
case 0x800d5104:{if(!s.storeHalf||!s.storeHalf(s.user,(r[4]+uint32_t(120)),uint16_t(r[0])))return {};c.pc=0x800d5108;break;} // b0040078
case 0x800d5108:{r[3]=uint32_t(1);c.pc=0x800d510c;break;} // 38600001
case 0x800d510c:{c.pc=0x800d5554;break;} // 48000448
case 0x800d5110:{compare(c,0,signedWord(r[31]),122);c.pc=0x800d5114;break;} // 2c1f007a
case 0x800d5114:{c.pc=(true)&&((c.condition[2]==false))?0x800d5140:0x800d5118;break;} // 4082002c
case 0x800d5118:{if(!word((r[13]+uint32_t(7688)),r[4]))return {};c.pc=0x800d511c;break;} // 808d1e08
case 0x800d511c:{r[3]=uint32_t(131072);c.pc=0x800d5120;break;} // 3c600002
case 0x800d5120:{r[0]=r[3]+uint32_t(-31073);c.pc=0x800d5124;break;} // 3803869f
case 0x800d5124:{if(!word((r[4]+uint32_t(8)),r[3]))return {};c.pc=0x800d5128;break;} // 80640008
case 0x800d5128:{compare(c,0,signedWord(r[3]),signedWord(r[0]));c.pc=0x800d512c;break;} // 7c030000
case 0x800d512c:{c.pc=(true)&&((c.condition[0]==false))?0x800d5138:0x800d5130;break;} // 4080000c
case 0x800d5130:{r[0]=r[3]+uint32_t(1);c.pc=0x800d5134;break;} // 38030001
case 0x800d5134:{if(!store((r[4]+uint32_t(8)),r[0]))return {};c.pc=0x800d5138;break;} // 90040008
case 0x800d5138:{r[3]=uint32_t(1);c.pc=0x800d513c;break;} // 38600001
case 0x800d513c:{c.pc=0x800d5554;break;} // 48000418
case 0x800d5140:{compare(c,0,signedWord(r[31]),123);c.pc=0x800d5144;break;} // 2c1f007b
case 0x800d5144:{c.pc=(true)&&((c.condition[2]==false))?0x800d519c:0x800d5148;break;} // 40820058
case 0x800d5148:{if(!external(0x801410e8,0x800d514c))return {};break;} // 4806bfa1
case 0x800d514c:{if(!word((r[13]+uint32_t(7136)),r[5]))return {};c.pc=0x800d5150;break;} // 80ad1be0
case 0x800d5150:{uint32_t value;if(!half((r[5]+uint32_t(112)),value))return {};r[4]=signHalf(value);c.pc=0x800d5154;break;} // a8850070
case 0x800d5154:{uint32_t value;if(!half((r[5]+uint32_t(114)),value))return {};r[0]=signHalf(value);c.pc=0x800d5158;break;} // a8050072
case 0x800d5158:{compare(c,0,signedWord(r[4]),signedWord(r[0]));c.pc=0x800d515c;break;} // 7c040000
case 0x800d515c:{c.pc=(true)&&((c.condition[0]==false))?0x800d5168:0x800d5160;break;} // 4080000c
case 0x800d5160:{r[0]=r[4]+uint32_t(1);c.pc=0x800d5164;break;} // 38040001
case 0x800d5164:{if(!s.storeHalf||!s.storeHalf(s.user,(r[5]+uint32_t(112)),uint16_t(r[0])))return {};c.pc=0x800d5168;break;} // b0050070
case 0x800d5168:{compare(c,0,signedWord(r[3]),0);c.pc=0x800d516c;break;} // 2c030000
case 0x800d516c:{c.pc=(true)&&((c.condition[2]==true))?0x800d5194:0x800d5170;break;} // 41820028
case 0x800d5170:{r[0]=r[3]*uint32_t(14);c.pc=0x800d5174;break;} // 1c03000e
case 0x800d5174:{if(!word((r[13]+uint32_t(7136)),r[3]))return {};c.pc=0x800d5178;break;} // 806d1be0
case 0x800d5178:{r[4]=r[3]+r[0];c.pc=0x800d517c;break;} // 7c830214
case 0x800d517c:{uint32_t value;if(!half((r[4]+uint32_t(6)),value))return {};r[3]=signHalf(value);c.pc=0x800d5180;break;} // a8640006
case 0x800d5180:{uint32_t value;if(!half((r[4]+uint32_t(2)),value))return {};r[0]=signHalf(value);c.pc=0x800d5184;break;} // a8040002
case 0x800d5184:{compare(c,0,signedWord(r[3]),signedWord(r[0]));c.pc=0x800d5188;break;} // 7c030000
case 0x800d5188:{c.pc=(true)&&((c.condition[0]==false))?0x800d5194:0x800d518c;break;} // 4080000c
case 0x800d518c:{r[0]=r[3]+uint32_t(1);c.pc=0x800d5190;break;} // 38030001
case 0x800d5190:{if(!s.storeHalf||!s.storeHalf(s.user,(r[4]+uint32_t(6)),uint16_t(r[0])))return {};c.pc=0x800d5194;break;} // b0040006
case 0x800d5194:{r[3]=uint32_t(1);c.pc=0x800d5198;break;} // 38600001
case 0x800d5198:{c.pc=0x800d5554;break;} // 480003bc
case 0x800d519c:{compare(c,0,signedWord(r[31]),124);c.pc=0x800d51a0;break;} // 2c1f007c
case 0x800d51a0:{c.pc=(true)&&((c.condition[2]==false))?0x800d51c8:0x800d51a4;break;} // 40820028
case 0x800d51a4:{if(!word((r[13]+uint32_t(7136)),r[4]))return {};c.pc=0x800d51a8;break;} // 808d1be0
case 0x800d51a8:{uint32_t value;if(!half((r[4]+uint32_t(116)),value))return {};r[3]=signHalf(value);c.pc=0x800d51ac;break;} // a8640074
case 0x800d51ac:{uint32_t value;if(!half((r[4]+uint32_t(118)),value))return {};r[0]=signHalf(value);c.pc=0x800d51b0;break;} // a8040076
case 0x800d51b0:{compare(c,0,signedWord(r[3]),signedWord(r[0]));c.pc=0x800d51b4;break;} // 7c030000
case 0x800d51b4:{c.pc=(true)&&((c.condition[0]==false))?0x800d51c0:0x800d51b8;break;} // 4080000c
case 0x800d51b8:{r[0]=r[3]+uint32_t(1);c.pc=0x800d51bc;break;} // 38030001
case 0x800d51bc:{if(!s.storeHalf||!s.storeHalf(s.user,(r[4]+uint32_t(116)),uint16_t(r[0])))return {};c.pc=0x800d51c0;break;} // b0040074
case 0x800d51c0:{r[3]=uint32_t(1);c.pc=0x800d51c4;break;} // 38600001
case 0x800d51c4:{c.pc=0x800d5554;break;} // 48000390
case 0x800d51c8:{compare(c,0,signedWord(r[31]),125);c.pc=0x800d51cc;break;} // 2c1f007d
case 0x800d51cc:{c.pc=(true)&&((c.condition[2]==false))?0x800d51e8:0x800d51d0;break;} // 4082001c
case 0x800d51d0:{if(!word((r[13]+uint32_t(7136)),r[5]))return {};c.pc=0x800d51d4;break;} // 80ad1be0
case 0x800d51d4:{r[3]=uint32_t(1);c.pc=0x800d51d8;break;} // 38600001
case 0x800d51d8:{uint32_t value;if(!half((r[5]+uint32_t(154)),value))return {};r[4]=signHalf(value);c.pc=0x800d51dc;break;} // a885009a
case 0x800d51dc:{r[0]=r[4]+uint32_t(1);c.pc=0x800d51e0;break;} // 38040001
case 0x800d51e0:{if(!s.storeHalf||!s.storeHalf(s.user,(r[5]+uint32_t(154)),uint16_t(r[0])))return {};c.pc=0x800d51e4;break;} // b005009a
case 0x800d51e4:{c.pc=0x800d5554;break;} // 48000370
case 0x800d51e8:{compare(c,0,signedWord(r[31]),87);c.pc=0x800d51ec;break;} // 2c1f0057
case 0x800d51ec:{c.pc=(true)&&((c.condition[2]==false))?0x800d5208:0x800d51f0;break;} // 4082001c
case 0x800d51f0:{if(!word((r[13]+uint32_t(7136)),r[5]))return {};c.pc=0x800d51f4;break;} // 80ad1be0
case 0x800d51f4:{r[3]=uint32_t(1);c.pc=0x800d51f8;break;} // 38600001
case 0x800d51f8:{uint32_t value;if(!half((r[5]+uint32_t(156)),value))return {};r[4]=signHalf(value);c.pc=0x800d51fc;break;} // a885009c
case 0x800d51fc:{r[0]=r[4]+uint32_t(1);c.pc=0x800d5200;break;} // 38040001
case 0x800d5200:{if(!s.storeHalf||!s.storeHalf(s.user,(r[5]+uint32_t(156)),uint16_t(r[0])))return {};c.pc=0x800d5204;break;} // b005009c
case 0x800d5204:{c.pc=0x800d5554;break;} // 48000350
case 0x800d5208:{r[0]=r[31]+uint32_t(-6);c.pc=0x800d520c;break;} // 381ffffa
case 0x800d520c:{compare(c,0,uint64_t(r[0]),1);c.pc=0x800d5210;break;} // 28000001
case 0x800d5210:{c.pc=(true)&&((c.condition[1]==false))?0x800d521c:0x800d5214;break;} // 4081000c
case 0x800d5214:{compare(c,0,signedWord(r[31]),8);c.pc=0x800d5218;break;} // 2c1f0008
case 0x800d5218:{c.pc=(true)&&((c.condition[2]==false))?0x800d5270:0x800d521c;break;} // 40820058
case 0x800d521c:{compare(c,0,signedWord(r[31]),7);c.pc=0x800d5220;break;} // 2c1f0007
case 0x800d5220:{c.pc=(true)&&((c.condition[2]==true))?0x800d5250:0x800d5224;break;} // 41820030
case 0x800d5224:{c.pc=(true)&&((c.condition[0]==false))?0x800d5234:0x800d5228;break;} // 40800010
case 0x800d5228:{compare(c,0,signedWord(r[31]),6);c.pc=0x800d522c;break;} // 2c1f0006
case 0x800d522c:{c.pc=(true)&&((c.condition[0]==false))?0x800d5240:0x800d5230;break;} // 40800014
case 0x800d5230:{c.pc=0x800d5300;break;} // 480000d0
case 0x800d5234:{compare(c,0,signedWord(r[31]),9);c.pc=0x800d5238;break;} // 2c1f0009
case 0x800d5238:{c.pc=(true)&&((c.condition[0]==false))?0x800d5300:0x800d523c;break;} // 408000c8
case 0x800d523c:{c.pc=0x800d5260;break;} // 48000024
case 0x800d5240:{if(!word((r[13]+uint32_t(7136)),r[3]))return {};c.pc=0x800d5244;break;} // 806d1be0
case 0x800d5244:{r[0]=uint32_t(1);c.pc=0x800d5248;break;} // 38000001
case 0x800d5248:{if(!s.storeByte||!s.storeByte(s.user,(r[3]+uint32_t(152)),uint8_t(r[0])))return {};c.pc=0x800d524c;break;} // 98030098
case 0x800d524c:{c.pc=0x800d5300;break;} // 480000b4
case 0x800d5250:{if(!word((r[13]+uint32_t(7136)),r[3]))return {};c.pc=0x800d5254;break;} // 806d1be0
case 0x800d5254:{r[0]=uint32_t(2);c.pc=0x800d5258;break;} // 38000002
case 0x800d5258:{if(!s.storeByte||!s.storeByte(s.user,(r[3]+uint32_t(152)),uint8_t(r[0])))return {};c.pc=0x800d525c;break;} // 98030098
case 0x800d525c:{c.pc=0x800d5300;break;} // 480000a4
case 0x800d5260:{if(!word((r[13]+uint32_t(7136)),r[3]))return {};c.pc=0x800d5264;break;} // 806d1be0
case 0x800d5264:{r[0]=uint32_t(3);c.pc=0x800d5268;break;} // 38000003
case 0x800d5268:{if(!s.storeByte||!s.storeByte(s.user,(r[3]+uint32_t(152)),uint8_t(r[0])))return {};c.pc=0x800d526c;break;} // 98030098
case 0x800d526c:{c.pc=0x800d5300;break;} // 48000094
case 0x800d5270:{r[0]=r[31]+uint32_t(-9);c.pc=0x800d5274;break;} // 381ffff7
case 0x800d5274:{compare(c,0,uint64_t(r[0]),1);c.pc=0x800d5278;break;} // 28000001
case 0x800d5278:{c.pc=(true)&&((c.condition[1]==false))?0x800d5284:0x800d527c;break;} // 4081000c
case 0x800d527c:{compare(c,0,signedWord(r[31]),11);c.pc=0x800d5280;break;} // 2c1f000b
case 0x800d5280:{c.pc=(true)&&((c.condition[2]==false))?0x800d52d8:0x800d5284;break;} // 40820058
case 0x800d5284:{compare(c,0,signedWord(r[31]),10);c.pc=0x800d5288;break;} // 2c1f000a
case 0x800d5288:{c.pc=(true)&&((c.condition[2]==true))?0x800d52b8:0x800d528c;break;} // 41820030
case 0x800d528c:{c.pc=(true)&&((c.condition[0]==false))?0x800d529c:0x800d5290;break;} // 40800010
case 0x800d5290:{compare(c,0,signedWord(r[31]),9);c.pc=0x800d5294;break;} // 2c1f0009
case 0x800d5294:{c.pc=(true)&&((c.condition[0]==false))?0x800d52a8:0x800d5298;break;} // 40800014
case 0x800d5298:{c.pc=0x800d5300;break;} // 48000068
case 0x800d529c:{compare(c,0,signedWord(r[31]),12);c.pc=0x800d52a0;break;} // 2c1f000c
case 0x800d52a0:{c.pc=(true)&&((c.condition[0]==false))?0x800d5300:0x800d52a4;break;} // 40800060
case 0x800d52a4:{c.pc=0x800d52c8;break;} // 48000024
case 0x800d52a8:{if(!word((r[13]+uint32_t(7136)),r[3]))return {};c.pc=0x800d52ac;break;} // 806d1be0
case 0x800d52ac:{r[0]=uint32_t(1);c.pc=0x800d52b0;break;} // 38000001
case 0x800d52b0:{if(!s.storeByte||!s.storeByte(s.user,(r[3]+uint32_t(153)),uint8_t(r[0])))return {};c.pc=0x800d52b4;break;} // 98030099
case 0x800d52b4:{c.pc=0x800d5300;break;} // 4800004c
case 0x800d52b8:{if(!word((r[13]+uint32_t(7136)),r[3]))return {};c.pc=0x800d52bc;break;} // 806d1be0
case 0x800d52bc:{r[0]=uint32_t(2);c.pc=0x800d52c0;break;} // 38000002
case 0x800d52c0:{if(!s.storeByte||!s.storeByte(s.user,(r[3]+uint32_t(153)),uint8_t(r[0])))return {};c.pc=0x800d52c4;break;} // 98030099
case 0x800d52c4:{c.pc=0x800d5300;break;} // 4800003c
case 0x800d52c8:{if(!word((r[13]+uint32_t(7136)),r[3]))return {};c.pc=0x800d52cc;break;} // 806d1be0
case 0x800d52cc:{r[0]=uint32_t(3);c.pc=0x800d52d0;break;} // 38000003
case 0x800d52d0:{if(!s.storeByte||!s.storeByte(s.user,(r[3]+uint32_t(153)),uint8_t(r[0])))return {};c.pc=0x800d52d4;break;} // 98030099
case 0x800d52d4:{c.pc=0x800d5300;break;} // 4800002c
case 0x800d52d8:{compare(c,0,signedWord(r[31]),89);c.pc=0x800d52dc;break;} // 2c1f0059
case 0x800d52dc:{c.pc=(true)&&((c.condition[2]==false))?0x800d52e8:0x800d52e0;break;} // 4082000c
case 0x800d52e0:{r[3]=uint32_t(1);c.pc=0x800d52e4;break;} // 38600001
case 0x800d52e4:{c.pc=0x800d5554;break;} // 48000270
case 0x800d52e8:{compare(c,0,signedWord(r[31]),240);c.pc=0x800d52ec;break;} // 2c1f00f0
case 0x800d52ec:{c.pc=(true)&&((c.condition[0]==true))?0x800d5300:0x800d52f0;break;} // 41800014
case 0x800d52f0:{compare(c,0,signedWord(r[31]),339);c.pc=0x800d52f4;break;} // 2c1f0153
case 0x800d52f4:{c.pc=(true)&&((c.condition[0]==false))?0x800d5300:0x800d52f8;break;} // 4080000c
case 0x800d52f8:{r[3]=r[31]+uint32_t(-112);c.pc=0x800d52fc;break;} // 387fff90
case 0x800d52fc:{if(!external(0x800858fc,0x800d5300))return {};break;} // 4bfb0601
case 0x800d5300:{compare(c,0,signedWord(r[31]),121);c.pc=0x800d5304;break;} // 2c1f0079
case 0x800d5304:{r[4]=uint32_t(0);c.pc=0x800d5308;break;} // 38800000
case 0x800d5308:{r[29]=r[4]|r[4];c.pc=0x800d530c;break;} // 7c9d2378
case 0x800d530c:{r[30]=r[4]|r[4];c.pc=0x800d5310;break;} // 7c9e2378
case 0x800d5310:{c.pc=(true)&&((c.condition[0]==true))?0x800d5344:0x800d5314;break;} // 41800034
case 0x800d5314:{compare(c,0,signedWord(r[31]),236);c.pc=0x800d5318;break;} // 2c1f00ec
case 0x800d5318:{c.pc=(true)&&((c.condition[0]==false))?0x800d5344:0x800d531c;break;} // 4080002c
case 0x800d531c:{if(!word((r[13]+uint32_t(7136)),r[4]))return {};c.pc=0x800d5320;break;} // 808d1be0
case 0x800d5320:{r[3]=uint32_t(1);c.pc=0x800d5324;break;} // 38600001
case 0x800d5324:{r[29]=r[4]+uint32_t(402);c.pc=0x800d5328;break;} // 3ba40192
case 0x800d5328:{c.lr=0x800d532c;c.pc=0x800d4ed4;break;} // 4bfffbad
case 0x800d532c:{compare(c,0,signedWord(r[3]),0);c.pc=0x800d5330;break;} // 2c030000
case 0x800d5330:{r[4]=uint32_t(10);c.pc=0x800d5334;break;} // 3880000a
case 0x800d5334:{c.pc=(true)&&((c.condition[2]==true))?0x800d533c:0x800d5338;break;} // 41820008
case 0x800d5338:{r[4]=uint32_t(20);c.pc=0x800d533c;break;} // 38800014
case 0x800d533c:{r[0]=uint32_t(1);c.pc=0x800d5340;break;} // 38000001
case 0x800d5340:{c.pc=0x800d5398;break;} // 48000058
case 0x800d5344:{compare(c,0,signedWord(r[31]),1);c.pc=0x800d5348;break;} // 2c1f0001
case 0x800d5348:{c.pc=(true)&&((c.condition[0]==true))?0x800d536c:0x800d534c;break;} // 41800024
case 0x800d534c:{compare(c,0,signedWord(r[31]),121);c.pc=0x800d5350;break;} // 2c1f0079
case 0x800d5350:{c.pc=(true)&&((c.condition[0]==false))?0x800d536c:0x800d5354;break;} // 4080001c
case 0x800d5354:{if(!word((r[13]+uint32_t(7136)),r[3]))return {};c.pc=0x800d5358;break;} // 806d1be0
case 0x800d5358:{r[30]=uint32_t(0);c.pc=0x800d535c;break;} // 3bc00000
case 0x800d535c:{r[4]=uint32_t(121);c.pc=0x800d5360;break;} // 38800079
case 0x800d5360:{r[0]=uint32_t(1);c.pc=0x800d5364;break;} // 38000001
case 0x800d5364:{r[29]=r[3]+uint32_t(160);c.pc=0x800d5368;break;} // 3ba300a0
case 0x800d5368:{c.pc=0x800d5398;break;} // 48000030
case 0x800d536c:{compare(c,0,signedWord(r[31]),240);c.pc=0x800d5370;break;} // 2c1f00f0
case 0x800d5370:{c.pc=(true)&&((c.condition[0]==true))?0x800d5394:0x800d5374;break;} // 41800024
case 0x800d5374:{compare(c,0,signedWord(r[31]),339);c.pc=0x800d5378;break;} // 2c1f0153
case 0x800d5378:{c.pc=(true)&&((c.condition[0]==false))?0x800d5394:0x800d537c;break;} // 4080001c
case 0x800d537c:{if(!word((r[13]+uint32_t(7136)),r[3]))return {};c.pc=0x800d5380;break;} // 806d1be0
case 0x800d5380:{r[4]=uint32_t(200);c.pc=0x800d5384;break;} // 388000c8
case 0x800d5384:{r[0]=uint32_t(1);c.pc=0x800d5388;break;} // 38000001
case 0x800d5388:{r[29]=r[3]+uint32_t(506);c.pc=0x800d538c;break;} // 3ba301fa
case 0x800d538c:{r[30]=r[3]+uint32_t(906);c.pc=0x800d5390;break;} // 3bc3038a
case 0x800d5390:{c.pc=0x800d5398;break;} // 48000008
case 0x800d5394:{r[0]=uint32_t(0);c.pc=0x800d5398;break;} // 38000000
case 0x800d5398:{compare(c,0,signedWord(r[0]),0);c.pc=0x800d539c;break;} // 2c000000
case 0x800d539c:{c.pc=(true)&&((c.condition[2]==false))?0x800d53a8:0x800d53a0;break;} // 4082000c
case 0x800d53a0:{r[3]=uint32_t(0);c.pc=0x800d53a4;break;} // 38600000
case 0x800d53a4:{c.pc=0x800d5554;break;} // 480001b0
case 0x800d53a8:{const uint64_t value=uint64_t(r[4])+uint32_t(-1);c.carry=value>>32;r[4]=uint32_t(value);compare(c,0,signedWord(r[4]),0);c.pc=0x800d53ac;break;} // 3484ffff
case 0x800d53ac:{r[3]=rotate(r[4],1)&0xfffffffe;c.pc=0x800d53b0;break;} // 5483083c
case 0x800d53b0:{r[0]=r[4]+uint32_t(1);c.pc=0x800d53b4;break;} // 38040001
case 0x800d53b4:{r[3]=r[29]+r[3];c.pc=0x800d53b8;break;} // 7c7d1a14
case 0x800d53b8:{c.ctr=r[0];c.pc=0x800d53bc;break;} // 7c0903a6
case 0x800d53bc:{c.pc=(true)&&((c.condition[0]==true))?0x800d5550:0x800d53c0;break;} // 41800194
case 0x800d53c0:{uint32_t value;if(!half((r[3]+uint32_t(0)),value))return {};r[0]=signHalf(value);c.pc=0x800d53c4;break;} // a8030000
case 0x800d53c4:{compare(c,0,signedWord(r[0]),0);c.pc=0x800d53c8;break;} // 2c000000
case 0x800d53c8:{c.pc=(true)&&((c.condition[2]==false))?0x800d5544:0x800d53cc;break;} // 4082017c
case 0x800d53cc:{compare(c,0,uint64_t(r[30]),0);c.pc=0x800d53d0;break;} // 281e0000
case 0x800d53d0:{r[0]=rotate(r[4],1)&0xfffffffe;c.pc=0x800d53d4;break;} // 5480083c
case 0x800d53d4:{r[5]=r[29]+r[0];c.pc=0x800d53d8;break;} // 7cbd0214
case 0x800d53d8:{r[3]=r[30]+r[0];c.pc=0x800d53dc;break;} // 7c7e0214
case 0x800d53dc:{c.pc=(true)&&((c.condition[2]==true))?0x800d54ac:0x800d53e0;break;} // 418200d0
case 0x800d53e0:{compare(c,0,signedWord(r[4]),0);c.pc=0x800d53e4;break;} // 2c040000
case 0x800d53e4:{c.pc=(true)&&((c.condition[1]==false))?0x800d5528:0x800d53e8;break;} // 40810144
case 0x800d53e8:{r[0]=rotate(r[4],29)&0x1fffffff;compare(c,0,signedWord(r[0]),0);c.pc=0x800d53ec;break;} // 5480e8ff
case 0x800d53ec:{c.ctr=r[0];c.pc=0x800d53f0;break;} // 7c0903a6
case 0x800d53f0:{c.pc=(true)&&((c.condition[2]==true))?0x800d5488:0x800d53f4;break;} // 41820098
case 0x800d53f4:{uint32_t value;if(!half((r[5]+uint32_t(-2)),value))return {};r[0]=signHalf(value);c.pc=0x800d53f8;break;} // a805fffe
case 0x800d53f8:{if(!s.storeHalf||!s.storeHalf(s.user,(r[5]+uint32_t(0)),uint16_t(r[0])))return {};c.pc=0x800d53fc;break;} // b0050000
case 0x800d53fc:{uint32_t value;if(!half((r[3]+uint32_t(-2)),value))return {};r[0]=signHalf(value);c.pc=0x800d5400;break;} // a803fffe
case 0x800d5400:{if(!s.storeHalf||!s.storeHalf(s.user,(r[3]+uint32_t(0)),uint16_t(r[0])))return {};c.pc=0x800d5404;break;} // b0030000
case 0x800d5404:{uint32_t value;if(!half((r[5]+uint32_t(-4)),value))return {};r[0]=signHalf(value);c.pc=0x800d5408;break;} // a805fffc
case 0x800d5408:{if(!s.storeHalf||!s.storeHalf(s.user,(r[5]+uint32_t(-2)),uint16_t(r[0])))return {};c.pc=0x800d540c;break;} // b005fffe
case 0x800d540c:{uint32_t value;if(!half((r[3]+uint32_t(-4)),value))return {};r[0]=signHalf(value);c.pc=0x800d5410;break;} // a803fffc
case 0x800d5410:{if(!s.storeHalf||!s.storeHalf(s.user,(r[3]+uint32_t(-2)),uint16_t(r[0])))return {};c.pc=0x800d5414;break;} // b003fffe
case 0x800d5414:{uint32_t value;if(!half((r[5]+uint32_t(-6)),value))return {};r[0]=signHalf(value);c.pc=0x800d5418;break;} // a805fffa
case 0x800d5418:{if(!s.storeHalf||!s.storeHalf(s.user,(r[5]+uint32_t(-4)),uint16_t(r[0])))return {};c.pc=0x800d541c;break;} // b005fffc
case 0x800d541c:{uint32_t value;if(!half((r[3]+uint32_t(-6)),value))return {};r[0]=signHalf(value);c.pc=0x800d5420;break;} // a803fffa
case 0x800d5420:{if(!s.storeHalf||!s.storeHalf(s.user,(r[3]+uint32_t(-4)),uint16_t(r[0])))return {};c.pc=0x800d5424;break;} // b003fffc
case 0x800d5424:{uint32_t value;if(!half((r[5]+uint32_t(-8)),value))return {};r[0]=signHalf(value);c.pc=0x800d5428;break;} // a805fff8
case 0x800d5428:{if(!s.storeHalf||!s.storeHalf(s.user,(r[5]+uint32_t(-6)),uint16_t(r[0])))return {};c.pc=0x800d542c;break;} // b005fffa
case 0x800d542c:{uint32_t value;if(!half((r[3]+uint32_t(-8)),value))return {};r[0]=signHalf(value);c.pc=0x800d5430;break;} // a803fff8
case 0x800d5430:{if(!s.storeHalf||!s.storeHalf(s.user,(r[3]+uint32_t(-6)),uint16_t(r[0])))return {};c.pc=0x800d5434;break;} // b003fffa
case 0x800d5434:{uint32_t value;if(!half((r[5]+uint32_t(-10)),value))return {};r[0]=signHalf(value);c.pc=0x800d5438;break;} // a805fff6
case 0x800d5438:{if(!s.storeHalf||!s.storeHalf(s.user,(r[5]+uint32_t(-8)),uint16_t(r[0])))return {};c.pc=0x800d543c;break;} // b005fff8
case 0x800d543c:{uint32_t value;if(!half((r[3]+uint32_t(-10)),value))return {};r[0]=signHalf(value);c.pc=0x800d5440;break;} // a803fff6
case 0x800d5440:{if(!s.storeHalf||!s.storeHalf(s.user,(r[3]+uint32_t(-8)),uint16_t(r[0])))return {};c.pc=0x800d5444;break;} // b003fff8
case 0x800d5444:{uint32_t value;if(!half((r[5]+uint32_t(-12)),value))return {};r[0]=signHalf(value);c.pc=0x800d5448;break;} // a805fff4
case 0x800d5448:{if(!s.storeHalf||!s.storeHalf(s.user,(r[5]+uint32_t(-10)),uint16_t(r[0])))return {};c.pc=0x800d544c;break;} // b005fff6
case 0x800d544c:{uint32_t value;if(!half((r[3]+uint32_t(-12)),value))return {};r[0]=signHalf(value);c.pc=0x800d5450;break;} // a803fff4
case 0x800d5450:{if(!s.storeHalf||!s.storeHalf(s.user,(r[3]+uint32_t(-10)),uint16_t(r[0])))return {};c.pc=0x800d5454;break;} // b003fff6
case 0x800d5454:{uint32_t value;if(!half((r[5]+uint32_t(-14)),value))return {};r[0]=signHalf(value);c.pc=0x800d5458;break;} // a805fff2
case 0x800d5458:{if(!s.storeHalf||!s.storeHalf(s.user,(r[5]+uint32_t(-12)),uint16_t(r[0])))return {};c.pc=0x800d545c;break;} // b005fff4
case 0x800d545c:{uint32_t value;if(!half((r[3]+uint32_t(-14)),value))return {};r[0]=signHalf(value);c.pc=0x800d5460;break;} // a803fff2
case 0x800d5460:{if(!s.storeHalf||!s.storeHalf(s.user,(r[3]+uint32_t(-12)),uint16_t(r[0])))return {};c.pc=0x800d5464;break;} // b003fff4
case 0x800d5464:{uint32_t value;if(!half((r[5]+uint32_t(-16)),value))return {};r[0]=signHalf(value);c.pc=0x800d5468;break;} // a805fff0
case 0x800d5468:{if(!s.storeHalf||!s.storeHalf(s.user,(r[5]+uint32_t(-14)),uint16_t(r[0])))return {};c.pc=0x800d546c;break;} // b005fff2
case 0x800d546c:{r[5]=r[5]+uint32_t(-16);c.pc=0x800d5470;break;} // 38a5fff0
case 0x800d5470:{uint32_t value;if(!half((r[3]+uint32_t(-16)),value))return {};r[0]=signHalf(value);c.pc=0x800d5474;break;} // a803fff0
case 0x800d5474:{if(!s.storeHalf||!s.storeHalf(s.user,(r[3]+uint32_t(-14)),uint16_t(r[0])))return {};c.pc=0x800d5478;break;} // b003fff2
case 0x800d5478:{r[3]=r[3]+uint32_t(-16);c.pc=0x800d547c;break;} // 3863fff0
case 0x800d547c:{--c.ctr;c.pc=(c.ctr!=0)&&(true)?0x800d53f4:0x800d5480;break;} // 4200ff78
case 0x800d5480:{r[4]=r[4]&0x00000007;compare(c,0,signedWord(r[4]),0);c.pc=0x800d5484;break;} // 70840007
case 0x800d5484:{c.pc=(true)&&((c.condition[2]==true))?0x800d5528:0x800d5488;break;} // 418200a4
case 0x800d5488:{c.ctr=r[4];c.pc=0x800d548c;break;} // 7c8903a6
case 0x800d548c:{uint32_t value;if(!half((r[5]+uint32_t(-2)),value))return {};r[0]=signHalf(value);c.pc=0x800d5490;break;} // a805fffe
case 0x800d5490:{if(!s.storeHalf||!s.storeHalf(s.user,(r[5]+uint32_t(0)),uint16_t(r[0])))return {};c.pc=0x800d5494;break;} // b0050000
case 0x800d5494:{r[5]=r[5]+uint32_t(-2);c.pc=0x800d5498;break;} // 38a5fffe
case 0x800d5498:{uint32_t value;if(!half((r[3]+uint32_t(-2)),value))return {};r[0]=signHalf(value);c.pc=0x800d549c;break;} // a803fffe
case 0x800d549c:{if(!s.storeHalf||!s.storeHalf(s.user,(r[3]+uint32_t(0)),uint16_t(r[0])))return {};c.pc=0x800d54a0;break;} // b0030000
case 0x800d54a0:{r[3]=r[3]+uint32_t(-2);c.pc=0x800d54a4;break;} // 3863fffe
case 0x800d54a4:{--c.ctr;c.pc=(c.ctr!=0)&&(true)?0x800d548c:0x800d54a8;break;} // 4200ffe8
case 0x800d54a8:{c.pc=0x800d5528;break;} // 48000080
case 0x800d54ac:{compare(c,0,signedWord(r[4]),0);c.pc=0x800d54b0;break;} // 2c040000
case 0x800d54b0:{r[3]=r[4]|r[4];c.pc=0x800d54b4;break;} // 7c832378
case 0x800d54b4:{c.pc=(true)&&((c.condition[1]==false))?0x800d5528:0x800d54b8;break;} // 40810074
case 0x800d54b8:{r[0]=rotate(r[4],29)&0x1fffffff;compare(c,0,signedWord(r[0]),0);c.pc=0x800d54bc;break;} // 5480e8ff
case 0x800d54bc:{c.ctr=r[0];c.pc=0x800d54c0;break;} // 7c0903a6
case 0x800d54c0:{c.pc=(true)&&((c.condition[2]==true))?0x800d5514:0x800d54c4;break;} // 41820054
case 0x800d54c4:{uint32_t value;if(!half((r[5]+uint32_t(-2)),value))return {};r[0]=signHalf(value);c.pc=0x800d54c8;break;} // a805fffe
case 0x800d54c8:{if(!s.storeHalf||!s.storeHalf(s.user,(r[5]+uint32_t(0)),uint16_t(r[0])))return {};c.pc=0x800d54cc;break;} // b0050000
case 0x800d54cc:{uint32_t value;if(!half((r[5]+uint32_t(-4)),value))return {};r[0]=signHalf(value);c.pc=0x800d54d0;break;} // a805fffc
case 0x800d54d0:{if(!s.storeHalf||!s.storeHalf(s.user,(r[5]+uint32_t(-2)),uint16_t(r[0])))return {};c.pc=0x800d54d4;break;} // b005fffe
case 0x800d54d4:{uint32_t value;if(!half((r[5]+uint32_t(-6)),value))return {};r[0]=signHalf(value);c.pc=0x800d54d8;break;} // a805fffa
case 0x800d54d8:{if(!s.storeHalf||!s.storeHalf(s.user,(r[5]+uint32_t(-4)),uint16_t(r[0])))return {};c.pc=0x800d54dc;break;} // b005fffc
case 0x800d54dc:{uint32_t value;if(!half((r[5]+uint32_t(-8)),value))return {};r[0]=signHalf(value);c.pc=0x800d54e0;break;} // a805fff8
case 0x800d54e0:{if(!s.storeHalf||!s.storeHalf(s.user,(r[5]+uint32_t(-6)),uint16_t(r[0])))return {};c.pc=0x800d54e4;break;} // b005fffa
case 0x800d54e4:{uint32_t value;if(!half((r[5]+uint32_t(-10)),value))return {};r[0]=signHalf(value);c.pc=0x800d54e8;break;} // a805fff6
case 0x800d54e8:{if(!s.storeHalf||!s.storeHalf(s.user,(r[5]+uint32_t(-8)),uint16_t(r[0])))return {};c.pc=0x800d54ec;break;} // b005fff8
case 0x800d54ec:{uint32_t value;if(!half((r[5]+uint32_t(-12)),value))return {};r[0]=signHalf(value);c.pc=0x800d54f0;break;} // a805fff4
case 0x800d54f0:{if(!s.storeHalf||!s.storeHalf(s.user,(r[5]+uint32_t(-10)),uint16_t(r[0])))return {};c.pc=0x800d54f4;break;} // b005fff6
case 0x800d54f4:{uint32_t value;if(!half((r[5]+uint32_t(-14)),value))return {};r[0]=signHalf(value);c.pc=0x800d54f8;break;} // a805fff2
case 0x800d54f8:{if(!s.storeHalf||!s.storeHalf(s.user,(r[5]+uint32_t(-12)),uint16_t(r[0])))return {};c.pc=0x800d54fc;break;} // b005fff4
case 0x800d54fc:{uint32_t value;if(!half((r[5]+uint32_t(-16)),value))return {};r[0]=signHalf(value);c.pc=0x800d5500;break;} // a805fff0
case 0x800d5500:{if(!s.storeHalf||!s.storeHalf(s.user,(r[5]+uint32_t(-14)),uint16_t(r[0])))return {};c.pc=0x800d5504;break;} // b005fff2
case 0x800d5504:{r[5]=r[5]+uint32_t(-16);c.pc=0x800d5508;break;} // 38a5fff0
case 0x800d5508:{--c.ctr;c.pc=(c.ctr!=0)&&(true)?0x800d54c4:0x800d550c;break;} // 4200ffbc
case 0x800d550c:{r[3]=r[3]&0x00000007;compare(c,0,signedWord(r[3]),0);c.pc=0x800d5510;break;} // 70630007
case 0x800d5510:{c.pc=(true)&&((c.condition[2]==true))?0x800d5528:0x800d5514;break;} // 41820018
case 0x800d5514:{c.ctr=r[3];c.pc=0x800d5518;break;} // 7c6903a6
case 0x800d5518:{uint32_t value;if(!half((r[5]+uint32_t(-2)),value))return {};r[0]=signHalf(value);c.pc=0x800d551c;break;} // a805fffe
case 0x800d551c:{if(!s.storeHalf||!s.storeHalf(s.user,(r[5]+uint32_t(0)),uint16_t(r[0])))return {};c.pc=0x800d5520;break;} // b0050000
case 0x800d5520:{r[5]=r[5]+uint32_t(-2);c.pc=0x800d5524;break;} // 38a5fffe
case 0x800d5524:{--c.ctr;c.pc=(c.ctr!=0)&&(true)?0x800d5518:0x800d5528;break;} // 4200fff4
case 0x800d5528:{compare(c,0,uint64_t(r[30]),0);c.pc=0x800d552c;break;} // 281e0000
case 0x800d552c:{if(!s.storeHalf||!s.storeHalf(s.user,(r[29]+uint32_t(0)),uint16_t(r[31])))return {};c.pc=0x800d5530;break;} // b3fd0000
case 0x800d5530:{c.pc=(true)&&((c.condition[2]==true))?0x800d553c:0x800d5534;break;} // 4182000c
case 0x800d5534:{r[0]=uint32_t(0);c.pc=0x800d5538;break;} // 38000000
case 0x800d5538:{if(!s.storeHalf||!s.storeHalf(s.user,(r[30]+uint32_t(0)),uint16_t(r[0])))return {};c.pc=0x800d553c;break;} // b01e0000
case 0x800d553c:{r[3]=uint32_t(1);c.pc=0x800d5540;break;} // 38600001
case 0x800d5540:{c.pc=0x800d5554;break;} // 48000014
case 0x800d5544:{r[3]=r[3]+uint32_t(-2);c.pc=0x800d5548;break;} // 3863fffe
case 0x800d5548:{r[4]=r[4]+uint32_t(-1);c.pc=0x800d554c;break;} // 3884ffff
case 0x800d554c:{--c.ctr;c.pc=(c.ctr!=0)&&(true)?0x800d53c0:0x800d5550;break;} // 4200fe74
case 0x800d5550:{r[3]=uint32_t(0);c.pc=0x800d5554;break;} // 38600000
case 0x800d5554:{if(!word((r[1]+uint32_t(36)),r[0]))return {};c.pc=0x800d5558;break;} // 80010024
case 0x800d5558:{if(!word((r[1]+uint32_t(28)),r[31]))return {};c.pc=0x800d555c;break;} // 83e1001c
case 0x800d555c:{if(!word((r[1]+uint32_t(24)),r[30]))return {};c.pc=0x800d5560;break;} // 83c10018
case 0x800d5560:{if(!word((r[1]+uint32_t(20)),r[29]))return {};c.pc=0x800d5564;break;} // 83a10014
case 0x800d5564:{c.lr=r[0];c.pc=0x800d5568;break;} // 7c0803a6
case 0x800d5568:{r[1]=r[1]+uint32_t(32);c.pc=0x800d556c;break;} // 38210020
case 0x800d556c:{c.pc=c.lr;break;} // 4e800020
// partyInit 800bd6ac/2c SHA256 1de59625edee5097ee19b46d0c8e22fcf80c6e62fe1c9ac3ad0f621092ece667
case 0x800bd6ac:{const auto target=(r[1]+uint32_t(-16));if(!store(target,r[1]))return {};r[1]=target;c.pc=0x800bd6b0;break;} // 9421fff0
case 0x800bd6b0:{r[0]=c.lr;c.pc=0x800bd6b4;break;} // 7c0802a6
case 0x800bd6b4:{r[3]=r[13]+uint32_t(7096);c.pc=0x800bd6b8;break;} // 386d1bb8
case 0x800bd6b8:{r[4]=uint32_t(0);c.pc=0x800bd6bc;break;} // 38800000
case 0x800bd6bc:{if(!store((r[1]+uint32_t(20)),r[0]))return {};c.pc=0x800bd6c0;break;} // 90010014
case 0x800bd6c0:{r[5]=uint32_t(8);c.pc=0x800bd6c4;break;} // 38a00008
case 0x800bd6c4:{if(!external(0x800050b4,0x800bd6c8))return {};break;} // 4bf479f1
case 0x800bd6c8:{if(!word((r[1]+uint32_t(20)),r[0]))return {};c.pc=0x800bd6cc;break;} // 80010014
case 0x800bd6cc:{c.lr=r[0];c.pc=0x800bd6d0;break;} // 7c0803a6
case 0x800bd6d0:{r[1]=r[1]+uint32_t(16);c.pc=0x800bd6d4;break;} // 38210010
case 0x800bd6d4:{c.pc=c.lr;break;} // 4e800020
