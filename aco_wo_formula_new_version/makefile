# first makefile under AntColonyOptimization folder will be run like make clean && make run

ARGS := arg1

run_wo:
	cd map_draw && rm -rf *.csv
	cd AntColonyOptimization_Without_Formula && make clean && make run ARGS="$(ARGS)"
	source venv/bin/activate && cd map_draw && python3 optimize_routes.py && python3 rota_cizim.py without_formula

run_with:
	cd map_draw && rm -rf *.csv
	cd AntColonyOptimization_With_Formula && make clean && make run ARGS="$(ARGS)"
	source venv/bin/activate && cd map_draw && python3 optimize_routes.py && python3 rota_cizim.py with_formula

install_pip:
	cd map_draw && pip3 install -r requirements.txt

run_all:
	cd map_draw && rm -rf *.csv && rm -rf *.png && rm -rf *.html
	make run_wo && make run_with