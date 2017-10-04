VERSION="2.6"
RELEASE="1"

sudo checkinstall 	--nodoc \
					--pkgname 'frameinspector' \
					--pkgversion ${VERSION} \
					--pkgrelease ${RELEASE} \
					--pkggroup development \
					--maintainer "Michael Uman" \
					--strip \
					-y \
					make -f Makefile.installer install

