# Maintainer: Your Name <youremail@domain.com>
pkgname=tic-tac-toe
pkgver=1.0.0
pkgrel=1
pkgdesc="A terminal tic-tac-toe app"
arch=('any')
url="https://github.com/David-Vella/tic-tac-toe"
license=('MIT')
depends=('gcc' 'ncurses')
makedepends=('make')
source=("git+https://github.com/David-Vella/tic-tac-toe.git")
md5sums=('SKIP')

build() {
	cd "${srcdir}/${pkgname}/"
	make
}

package() {
	cd "${srcdir}/${pkgname}/"
	make DESTDIR="$pkgdir" install
}