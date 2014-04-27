intValue 		= 100;
doubleValue 	= 100.123456;
stringValue 	= "string";
boolValue		= true;

function testFunc(a, b, ...)
	print("a: ", a, "b: ", b);
	print("intValue: ", intValue);
	print("doubleValue: ", doubleValue);
	print("stringValue: ", stringValue);
	print("boolValue: ", boolValue);
end

tbl = {
	a = 10,
	b = 20;
	c = "100",
	d = "200",
	e = true,
	f = function() print("table member"); end;
};

function testTablePrint(...)
	--print(fmt);
	print(arg);
	--io.write(string.format(fmt, unpack(arg)));
end