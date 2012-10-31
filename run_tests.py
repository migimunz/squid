import subprocess
import glob
import os

print('')
for test in glob.glob('test/*.t.hpp'):
	test = test.replace('.t.hpp', '_test.exe')
	if os.path.exists(test):
		print('Running %s:' % test)
		subprocess.call(test)
		print('')

if 'GRAPHVIZ_PATH' in os.environ:
	print('Generating AST graphs...')
	for dot in glob.glob('test/*.dot'):
		pngname = dot.replace('.dot', '.png')
		print('Generated %s' % pngname)
		subprocess.call(['dot', dot, '-Tpng', '-o'+pngname])
	print('')

