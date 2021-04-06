
fact = 2^(1/12);

start_C = 3000;

dist = [0 2 2 1 2 2 2];
names = 'CDEFGAB';
cdist = cumsum(dist);

for k = 1:7
    fprintf('#define %s %.0f\n', names(k), start_C / fact^cdist(k));
end

%%

25 * 1.15 * 8 * 5 * 52

