

filepath = '../../test.edf';

if exist(filepath,'file')
	header = edf(filepath);

	disp(header)
end