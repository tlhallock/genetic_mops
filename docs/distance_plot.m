hold off

npoints = 52;


f = @(x) (sin(pi*x))

t = linspace(0,1,npoints);

p1 = zeros(1, npoints);
p2 = [linspace(0, 1, npoints / 2) linspace(1, 0,  npoints / 2)];
p3 = [linspace(0, f(.25),  npoints / 4) linspace(f(.25), f(.5),  npoints / 4) linspace(f(.5), f(.75), npoints / 4) linspace(f(.75), f(1), npoints / 4)];



plot(t, f(t), 'k', t, p1, 'b', t, p2, 'r', t, p3, 'g')
axis([-.25 1.25 -.25 1.25])
print('aplot.png')
