import os
import glob;

env = Environment()
env.SConscript('lib/SConscript')
env.SConscript('test/SConscript')

## Clean .dot files and graphs
if env.GetOption('clean'):
	files = glob.glob('test/*.dot') + glob.glob('test/*.png')
	for fname in files:
		os.remove(fname)