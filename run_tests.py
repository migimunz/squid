import subprocess
import glob
print('')
for test in glob.glob('test/*.t.hpp'):
	test = test.replace('.t.hpp', '_test.exe')
	print('Running %s:' % test)
	subprocess.call(test)
	print('')
