#!/bin/sh

epoch=0
pkgname=`awk '/Package:/ {print $2}' debian/control`
version=`awk '/Version:/ {print $2}' debian/control`
pkgdir=tmp/$pkgname/$epoch/$version
bindir=$pkgdir/data/usr/bin

pmlnk.pm --copy --target $pkgdir/data/usr/share/perl5 .
mkdir $pkgdir/control
cp debian/control $pkgdir/control
echo '2.0' > $pkgdir/debian-binary

mkdir -p $bindir
cp md5cat/chkmd5.pl md5cat/cmp2md5.pl md5cat/mkcat.pl md5cat/xmd5.pl $bindir

mkdeb.pl $pkgdir

