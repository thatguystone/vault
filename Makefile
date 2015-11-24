all:

test:
	sudo nosetests3 --processes=-1 --with-coverage --cover-package=vault --process-timeout=20

install:
	python3 setup.py install --install-layout=deb --root=$(DESTDIR)

clean:
	sudo py3clean vault/
	sudo rm -f .coverage
	sudo rm -rf test_data/
