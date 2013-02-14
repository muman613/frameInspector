sudo checkinstall 	--nodoc \
					--pkgname 'frameinspector' \
					--pkgversion 1.24 \
					--pkgrelease 1 \
					--pkggroup development \
					--maintainer "Michael Uman" \
					--strip \
					-y \
					make -f Makefile.installer install

