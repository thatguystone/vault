all:

test:
	sudo nosetests3

clean:
	sudo py3clean vault/
	sudo rm -f .coverage
	sudo rm -rf test_data/
