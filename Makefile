test:
	sudo nosetests3 --processes=-1 --with-coverage --cover-package=vault

clean:
	sudo py3clean vault/
	sudo rm -f .coverage
	sudo rm -rf test_data/
