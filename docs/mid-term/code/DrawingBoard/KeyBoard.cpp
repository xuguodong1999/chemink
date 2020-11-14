#include "KeyBoard.h"
KeyBoard::KeyBoard(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	ui.label->adjustSize();
	ui.label->setGeometry(QRect(328, 240, 329, 27 * 4));
	ui.label->setWordWrap(true);
	ui.label->setAlignment(Qt::AlignTop);
	setWindowFlags(Qt::FramelessWindowHint);
	connect(ui.h, SIGNAL(clicked()), this, SLOT(h()));
	connect(ui.he, SIGNAL(clicked()), this, SLOT(he()));
	connect(ui.li, SIGNAL(clicked()), this, SLOT(li()));
	connect(ui.be, SIGNAL(clicked()), this, SLOT(be()));
	connect(ui.b, SIGNAL(clicked()), this, SLOT(b()));
	connect(ui.c, SIGNAL(clicked()), this, SLOT(c()));
	connect(ui.n, SIGNAL(clicked()), this, SLOT(n()));
	connect(ui.o, SIGNAL(clicked()), this, SLOT(o()));
	connect(ui.f, SIGNAL(clicked()), this, SLOT(f()));
	connect(ui.ne, SIGNAL(clicked()), this, SLOT(ne()));
	connect(ui.na, SIGNAL(clicked()), this, SLOT(na()));
	connect(ui.mg, SIGNAL(clicked()), this, SLOT(mg()));
	connect(ui.al, SIGNAL(clicked()), this, SLOT(al()));
	connect(ui.si, SIGNAL(clicked()), this, SLOT(si()));
	connect(ui.p, SIGNAL(clicked()), this, SLOT(p()));
	connect(ui.s, SIGNAL(clicked()), this, SLOT(s()));
	connect(ui.cl, SIGNAL(clicked()), this, SLOT(cl()));
	connect(ui.ar, SIGNAL(clicked()), this, SLOT(ar()));
	connect(ui.k, SIGNAL(clicked()), this, SLOT(k()));
	connect(ui.ca, SIGNAL(clicked()), this, SLOT(ca()));
	connect(ui.sc, SIGNAL(clicked()), this, SLOT(sc()));
	connect(ui.ti, SIGNAL(clicked()), this, SLOT(ti()));
	connect(ui.v, SIGNAL(clicked()), this, SLOT(v()));
	connect(ui.cr, SIGNAL(clicked()), this, SLOT(cr()));
	connect(ui.mn, SIGNAL(clicked()), this, SLOT(mn()));
	connect(ui.fe, SIGNAL(clicked()), this, SLOT(fe()));
	connect(ui.co, SIGNAL(clicked()), this, SLOT(co()));
	connect(ui.ni, SIGNAL(clicked()), this, SLOT(ni()));
	connect(ui.cu, SIGNAL(clicked()), this, SLOT(cu()));
	connect(ui.zn, SIGNAL(clicked()), this, SLOT(zn()));
	connect(ui.ga, SIGNAL(clicked()), this, SLOT(ga()));
	connect(ui.ge, SIGNAL(clicked()), this, SLOT(ge()));
	connect(ui.as, SIGNAL(clicked()), this, SLOT(as()));
	connect(ui.se, SIGNAL(clicked()), this, SLOT(se()));
	connect(ui.br, SIGNAL(clicked()), this, SLOT(br()));
	connect(ui.kr, SIGNAL(clicked()), this, SLOT(kr()));
	connect(ui.rb, SIGNAL(clicked()), this, SLOT(rb()));
	connect(ui.sr, SIGNAL(clicked()), this, SLOT(sr()));
	connect(ui.y, SIGNAL(clicked()), this, SLOT(y()));
	connect(ui.zr, SIGNAL(clicked()), this, SLOT(zr()));
	connect(ui.nb, SIGNAL(clicked()), this, SLOT(nb()));
	connect(ui.mo, SIGNAL(clicked()), this, SLOT(mo()));
	connect(ui.tc, SIGNAL(clicked()), this, SLOT(tc()));
	connect(ui.ru, SIGNAL(clicked()), this, SLOT(ru()));
	connect(ui.rh, SIGNAL(clicked()), this, SLOT(rh()));
	connect(ui.pd, SIGNAL(clicked()), this, SLOT(pd()));
	connect(ui.ag, SIGNAL(clicked()), this, SLOT(ag()));
	connect(ui.cd, SIGNAL(clicked()), this, SLOT(cd()));
	connect(ui.in, SIGNAL(clicked()), this, SLOT(in()));
	connect(ui.sn, SIGNAL(clicked()), this, SLOT(sn()));
	connect(ui.sb, SIGNAL(clicked()), this, SLOT(sb()));
	connect(ui.te, SIGNAL(clicked()), this, SLOT(te()));
	connect(ui.i, SIGNAL(clicked()), this, SLOT(i()));
	connect(ui.xe, SIGNAL(clicked()), this, SLOT(xe()));
	connect(ui.cs, SIGNAL(clicked()), this, SLOT(cs()));
	connect(ui.ba, SIGNAL(clicked()), this, SLOT(ba()));
	connect(ui.la, SIGNAL(clicked()), this, SLOT(la()));
	connect(ui.ce, SIGNAL(clicked()), this, SLOT(ce()));
	connect(ui.pr, SIGNAL(clicked()), this, SLOT(pr()));
	connect(ui.nd, SIGNAL(clicked()), this, SLOT(nd()));
	connect(ui.pm, SIGNAL(clicked()), this, SLOT(pm()));
	connect(ui.sm, SIGNAL(clicked()), this, SLOT(sm()));
	connect(ui.eu, SIGNAL(clicked()), this, SLOT(eu()));
	connect(ui.gd, SIGNAL(clicked()), this, SLOT(gd()));
	connect(ui.tb, SIGNAL(clicked()), this, SLOT(tb()));
	connect(ui.dy, SIGNAL(clicked()), this, SLOT(dy()));
	connect(ui.ho, SIGNAL(clicked()), this, SLOT(ho()));
	connect(ui.er, SIGNAL(clicked()), this, SLOT(er()));
	connect(ui.tm, SIGNAL(clicked()), this, SLOT(tm()));
	connect(ui.yb, SIGNAL(clicked()), this, SLOT(yb()));
	connect(ui.lu, SIGNAL(clicked()), this, SLOT(lu()));
	connect(ui.hf, SIGNAL(clicked()), this, SLOT(hf()));
	connect(ui.ta, SIGNAL(clicked()), this, SLOT(ta()));
	connect(ui.w, SIGNAL(clicked()), this, SLOT(w()));
	connect(ui.re, SIGNAL(clicked()), this, SLOT(re()));
	connect(ui.os, SIGNAL(clicked()), this, SLOT(os()));
	connect(ui.ir, SIGNAL(clicked()), this, SLOT(ir()));
	connect(ui.pt, SIGNAL(clicked()), this, SLOT(pt()));
	connect(ui.au, SIGNAL(clicked()), this, SLOT(au()));
	connect(ui.hg, SIGNAL(clicked()), this, SLOT(hg()));
	connect(ui.tl, SIGNAL(clicked()), this, SLOT(tl()));
	connect(ui.pb, SIGNAL(clicked()), this, SLOT(pb()));
	connect(ui.bi, SIGNAL(clicked()), this, SLOT(bi()));
	connect(ui.po, SIGNAL(clicked()), this, SLOT(po()));
	connect(ui.at, SIGNAL(clicked()), this, SLOT(at()));
	connect(ui.rn, SIGNAL(clicked()), this, SLOT(rn()));
	connect(ui.fr, SIGNAL(clicked()), this, SLOT(fr()));
	connect(ui.ra, SIGNAL(clicked()), this, SLOT(ra()));
	connect(ui.ac, SIGNAL(clicked()), this, SLOT(ac()));
	connect(ui.th, SIGNAL(clicked()), this, SLOT(th()));
	connect(ui.pa, SIGNAL(clicked()), this, SLOT(pa()));
	connect(ui.u, SIGNAL(clicked()), this, SLOT(u()));
	connect(ui.np, SIGNAL(clicked()), this, SLOT(np()));
	connect(ui.pu, SIGNAL(clicked()), this, SLOT(pu()));
	connect(ui.am, SIGNAL(clicked()), this, SLOT(am()));
	connect(ui.cm, SIGNAL(clicked()), this, SLOT(cm()));
	connect(ui.bk, SIGNAL(clicked()), this, SLOT(bk()));
	connect(ui.cf, SIGNAL(clicked()), this, SLOT(cf()));
	connect(ui.es, SIGNAL(clicked()), this, SLOT(es()));
	connect(ui.fm, SIGNAL(clicked()), this, SLOT(fm()));
	connect(ui.md, SIGNAL(clicked()), this, SLOT(md()));
	connect(ui.no, SIGNAL(clicked()), this, SLOT(no()));
	connect(ui.lr, SIGNAL(clicked()), this, SLOT(lr()));
	connect(ui.rf, SIGNAL(clicked()), this, SLOT(rf()));
	connect(ui.db, SIGNAL(clicked()), this, SLOT(db()));
	connect(ui.sg, SIGNAL(clicked()), this, SLOT(sg()));
	connect(ui.bh, SIGNAL(clicked()), this, SLOT(bh()));
	connect(ui.hs, SIGNAL(clicked()), this, SLOT(hs()));
	connect(ui.mt, SIGNAL(clicked()), this, SLOT(mt()));
	connect(ui.ds, SIGNAL(clicked()), this, SLOT(ds()));
	connect(ui.rg, SIGNAL(clicked()), this, SLOT(rg()));
	connect(ui.add, SIGNAL(clicked()), this, SLOT(add()));
	connect(ui.equals, SIGNAL(clicked()), this, SLOT(equals()));
	connect(ui.backspace, SIGNAL(clicked()), this, SLOT(backspace()));
	connect(ui.postive, SIGNAL(clicked()), this, SLOT(positive()));
	connect(ui.negative, SIGNAL(clicked()), this, SLOT(negative()));
	connect(ui.bracketLeft, SIGNAL(clicked()), this, SLOT(bracketLeft()));
	connect(ui.bracketRight, SIGNAL(clicked()), this, SLOT(bracketRight()));
	connect(ui.with, SIGNAL(clicked()), this, SLOT(with()));
	connect(ui.zero, SIGNAL(clicked()), this, SLOT(zero()));
	connect(ui.one, SIGNAL(clicked()), this, SLOT(one()));
	connect(ui.two, SIGNAL(clicked()), this, SLOT(two()));
	connect(ui.three, SIGNAL(clicked()), this, SLOT(three()));
	connect(ui.four, SIGNAL(clicked()), this, SLOT(four()));
	connect(ui.five, SIGNAL(clicked()), this, SLOT(five()));
	connect(ui.six, SIGNAL(clicked()), this, SLOT(six()));
	connect(ui.seven, SIGNAL(clicked()), this, SLOT(seven()));
	connect(ui.eight, SIGNAL(clicked()), this, SLOT(eight()));
	connect(ui.nine, SIGNAL(clicked()), this, SLOT(nine()));
}
KeyBoard::~KeyBoard()
{
	this->close();
}
void KeyBoard::returnDrawingBoard() {
	emit sendCloseSignalforKeyBoard();
}
void KeyBoard::h() {
	emit sendData("H");
}
void KeyBoard::he() {
	emit sendData("He");
}
void KeyBoard::li() {
	emit sendData("Li");
}
void KeyBoard::be() {
	emit sendData("Be");
}
void KeyBoard::b() {
	emit sendData("B");
}
void KeyBoard::c() {
	emit sendData("C");
}
void KeyBoard::n() {
	emit sendData("N");
}
void KeyBoard::o() {
	emit sendData("O");
}
void KeyBoard::f() {
	emit sendData("F");
}
void KeyBoard::ne() {
	emit sendData("Ne");
}
void KeyBoard::na() {
	emit sendData("Na");
}
void KeyBoard::mg() {
	emit sendData("Mg");
}
void KeyBoard::al() {
	emit sendData("Al");
}
void KeyBoard::si() {
	emit sendData("Si");
}
void KeyBoard::p() {
	emit sendData("P");
}
void KeyBoard::s() {
	emit sendData("S");
}
void KeyBoard::cl() {
	emit sendData("Cl");
}
void KeyBoard::ar() {
	emit sendData("Ar");
}
void KeyBoard::k() {
	emit sendData("K");
}
void KeyBoard::ca() {
	emit sendData("Ca");
}
void KeyBoard::sc() {
	emit sendData("Sc");
}
void KeyBoard::ti() {
	emit sendData("Ti");
}
void KeyBoard::v() {
	emit sendData("V");
}
void KeyBoard::cr() {
	emit sendData("Cr");
}
void KeyBoard::mn() {
	emit sendData("Mn");
}
void KeyBoard::fe() {
	emit sendData("Fe");
}
void KeyBoard::co() {
	emit sendData("Co");
}
void KeyBoard::ni() {
	emit sendData("Ni");
}
void KeyBoard::cu() {
	emit sendData("Cu");
}
void KeyBoard::zn() {
	emit sendData("Zn");
}
void KeyBoard::ga() {
	emit sendData("Ga");
}
void KeyBoard::ge() {
	emit sendData("Ge");
}
void KeyBoard::as() {
	emit sendData("As");
}
void KeyBoard::se() {
	emit sendData("Se");
}
void KeyBoard::br() {
	emit sendData("Br");
}
void KeyBoard::kr() {
	emit sendData("Kr");
}
void KeyBoard::rb() {
	emit sendData("Rb");
}
void KeyBoard::sr() {
	emit sendData("Sr");
}
void KeyBoard::y() {
	emit sendData("Y");
}
void KeyBoard::zr() {
	emit sendData("Zr");
}
void KeyBoard::nb() {
	emit sendData("Nb");
}
void KeyBoard::mo() {
	emit sendData("Mo");
}
void KeyBoard::tc() {
	emit sendData("Tc");
}
void KeyBoard::ru() {
	emit sendData("Ru");
}
void KeyBoard::rh() {
	emit sendData("Rh");
}
void KeyBoard::pd() {
	emit sendData("Pd");
}
void KeyBoard::ag() {
	emit sendData("Ag");
}
void KeyBoard::cd() {
	emit sendData("Cd");
}
void KeyBoard::in() {
	emit sendData("In");
}
void KeyBoard::sn() {
	emit sendData("Sn");
}
void KeyBoard::sb() {
	emit sendData("Sb");
}
void KeyBoard::te() {
	emit sendData("Te");
}
void KeyBoard::i() {
	emit sendData("I");
}
void KeyBoard::xe() {
	emit sendData("Xe");
}
void KeyBoard::cs() {
	emit sendData("Cs");
}
void KeyBoard::ba() {
	emit sendData("Ba");
}
void KeyBoard::la() {
	emit sendData("La");
}
void KeyBoard::ce() {
	emit sendData("Ce");
}
void KeyBoard::pr() {
	emit sendData("Pr");
}
void KeyBoard::nd() {
	emit sendData("Nd");
}
void KeyBoard::pm() {
	emit sendData("Pm");
}
void KeyBoard::sm() {
	emit sendData("Sm");
}
void KeyBoard::eu() {
	emit sendData("Eu");
}
void KeyBoard::gd() {
	emit sendData("Gd");
}
void KeyBoard::tb() {
	emit sendData("Tb");
}
void KeyBoard::dy() {
	emit sendData("Dy");
}
void KeyBoard::ho() {
	emit sendData("Ho");
}
void KeyBoard::er() {
	emit sendData("Er");
}
void KeyBoard::tm() {
	emit sendData("Tm");
}
void KeyBoard::yb() {
	emit sendData("Yb");
}
void KeyBoard::lu() {
	emit sendData("Lu");
}
void KeyBoard::hf() {
	emit sendData("Hf");
}
void KeyBoard::ta() {
	emit sendData("Ta");
}
void KeyBoard::w() {
	emit sendData("W");
}
void KeyBoard::re() {
	emit sendData("Re");
}
void KeyBoard::os() {
	emit sendData("Os");
}
void KeyBoard::ir() {
	emit sendData("Ir");
}
void KeyBoard::pt() {
	emit sendData("Pt");
}
void KeyBoard::au() {
	emit sendData("Au");
}
void KeyBoard::hg() {
	emit sendData("Hg");
}
void KeyBoard::tl() {
	emit sendData("Tl");
}
void KeyBoard::pb() {
	emit sendData("Pb");
}
void KeyBoard::bi() {
	emit sendData("Bi");
}
void KeyBoard::po() {
	emit sendData("Po");
}
void KeyBoard::at() {
	emit sendData("At");
}
void KeyBoard::rn() {
	emit sendData("Rn");
}
void KeyBoard::fr() {
	emit sendData("Fr");
}
void KeyBoard::ra() {
	emit sendData("Ra");
}
void KeyBoard::ac() {
	emit sendData("Ac");
}
void KeyBoard::th() {
	emit sendData("Th");
}
void KeyBoard::pa() {
	emit sendData("Pa");
}
void KeyBoard::u() {
	emit sendData("U");
}
void KeyBoard::np() {
	emit sendData("Np");
}
void KeyBoard::pu() {
	emit sendData("Pu");
}
void KeyBoard::am() {
	emit sendData("Am");
}
void KeyBoard::cm() {
	emit sendData("Cm");
}
void KeyBoard::bk() {
	emit sendData("Bk");
}
void KeyBoard::cf() {
	emit sendData("Cf");
}
void KeyBoard::es() {
	emit sendData("Es");
}
void KeyBoard::fm() {
	emit sendData("Fm");
}
void KeyBoard::md() {
	emit sendData("Md");
}
void KeyBoard::no() {
	emit sendData("No");
}
void KeyBoard::lr() {
	emit sendData("Lr");
}
void KeyBoard::rf() {
	emit sendData("Rf");
}
void KeyBoard::db() {
	emit sendData("Db");
}
void KeyBoard::sg() {
	emit sendData("Sg");
}
void KeyBoard::bh() {
	emit sendData("Bh");
}
void KeyBoard::hs() {
	emit sendData("Hs");
}
void KeyBoard::mt() {
	emit sendData("Mt");
}
void KeyBoard::ds() {
	emit sendData("Ds");
}
void KeyBoard::rg() {
	emit sendData("Rg");
}
void KeyBoard::add() {
	emit sendData("+");
}
void KeyBoard::equals() {
	emit sendData("=");
}
void KeyBoard::backspace() {
	emit sendData("←");
}
void KeyBoard::bracketLeft() {
	emit sendData("(");
}
void KeyBoard::bracketRight() {
	emit sendData(")");
}
void KeyBoard::positive() {
	emit sendData("@");
}
void KeyBoard::negative() {
	emit sendData("$");
}
void KeyBoard::with() {
	emit sendData(".");
}
void KeyBoard::zero() {
	emit sendData("0");
}
void KeyBoard::one() {
	emit sendData("1");
}
void KeyBoard::two() {
	emit sendData("2");
}
void KeyBoard::three() {
	emit sendData("3");
}
void KeyBoard::four() {
	emit sendData("4");
}
void KeyBoard::five() {
	emit sendData("5");
}
void KeyBoard::six() {
	emit sendData("6");
}
void KeyBoard::seven() {
	emit sendData("7");
}
void KeyBoard::eight() {
	emit sendData("8");
}
void KeyBoard::nine() {
	emit sendData("9");
}
