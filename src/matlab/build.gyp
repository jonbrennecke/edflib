{
	'targets': [
      {
        'target_name': 'edf',
        'type': 'shared_library',
        'defines' : [
        	'MATLAB_MEX_FILE',
        ],
        'sources': [
          'edfmat.cpp',
        ],
        'include_dirs': [
			'C:/Program Files (x86)/MATLAB/R2010a Student/bin/win32'
        ],
        'cflags': [
			'-Werror',
        ],
      },
    ],
}