

filepath = '../../test2.edf';

if exist(filepath,'file')
	[ header, records ] = edf(filepath);

	% records{1,1}(1:20);

end