a = array.new(1000);
print(a);
print(array.size(a));

for i = 1, 1000 do
	array.set(a, i, 1/i);
end

for i = 1, 1000 do
	print(array.get(a, i));
end