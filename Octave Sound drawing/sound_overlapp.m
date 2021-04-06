
fs = 48000;
ts = 1/fs;
ar = 240;
sp = fs/ar;
T = 15;

arc = @(t, r) r * sin(acos(((ts:ts:t)/t-0.5)*2)) * ar;
ellipse = @(t, d) [1; -1] * arc(t, d/2);
linD = @(deg, t) (ts:ts:t) * ar * tan(pi/180 * deg);
linP = @(y1, y2, t) (y1 + (ts:ts:t) * (y2-y1)/t) * ar;
fit = @(s, T, f) [nan(size(f, 1), round(s/ts)), f, nan(size(f, 1), round((T - s)/ts) - size(f, 2))];
angle = @(f, deg) f + (1:length(f)) * pi/180 / sp * deg;

I = @(t, H) [[1; -1] * H/2 * ar + linD(0, t); fit(t/2-t/20, t, linP(-H/2, H/2, t/10))];
O = @(t, H) ellipse(t, H);
V = @(t, H) [fit(0, t, linP(H/2, -H/2, t/2)); fit(t/2, t, linP(-H/2, H/2, t/2))];
L = @(t, H) [fit(0, t, linP(-H/2, H/2, t/10)); - H/2 * ar + linD(0, t)];
E = @(t, H) [fit(0, t, linP(-H/2, H/2, t/10)); [1; 0; -1] * H/2 * ar + linD(0, t)];
U = @(t, H) [fit(0, t, linP(H/2, 0, t/20)); fit(t-t/20, t, linP(0, H/2, t/20)); fit(t/20, t, -arc(t-t/10, H/2))];
Heart = @(t) (t/7 + [sqrt(1 - (abs((-1:(ts*2/t):1)*2) - 1).^2)/2; .6 * (acos(2*.7 - 1 - abs((-1:(ts*2/t):1)*2)*.7) - pi)]) * t/0.009;

A = @(t, H) [fit(0, t, linP(-H/2, H/2, t/2)); fit(t/2, t, linP(H/2, -H/2, t/2)); fit(t/4, t, linD(0, t/2))];
C = @(t, H) [1; -1] * H/2 * sin(acos(((ts:ts:t/1.2)/t-0.5)*2)) * ar;
D = @(t, H) [fit(0, t, linP(-H/2, H/2, t/10)); fit(0, t, [1; -1] * H/2 * sin(acos(0.5-((ts:ts:t/1.334)/t)*2)) * ar)];
F = @(t, H) [fit(0, t, linP(-H/2, H/2, t/10)); [1; 0] * H/2 * ar + linD(0, t)];
H = @(t, H) [fit(0, t, linP(-H/2, H/2, t/10)); linD(0, t); fit(t-t/10, t, linP(-H/2, H/2, t/10))];
% K

TEST = H;

FIT = @(s, f) fit(s, T, f);

f = 1500 + [...
%     300 + FIT(4, ellipse(1, 1))
%     300 + FIT(4, Heart(1))
    fit(2.0, T, I(.6, 1))
    fit(3.0, T, L(.8, 1))
    fit(3.6, T, O(.8, 1))
    fit(4.6, T, V(.8, 1))
    fit(5.5, T, E(.8, 1))
    fit(6.5, T, U(.8, 1))
    20 + fit(7.5, T, Heart(1.5))
%     200 + fit(9, T, linD(0, 5))
%     fit(10, T, C(1, 1))
%     fit(11, T, D(1, 1))
%     fit(12, T, TEST(1, 1))
    ];

% f = angle(f, 45);

% Simulate
figure(1)
plot(ts:ts:T, f', 'linewidth', 1); axis equal
set(gca, 'DataAspectRatio', [ts*fs*.0043 1 1])


%%
fade_win = 0.02;

f(isnan(f)) = 0;
y = 0;
fad = round(fade_win/ts);
z = hann(2*fad)'; z = z(1:fad);
fade = @(y) y .* [z ones(1, length(y) - 2*fad) fliplr(z)];

for k = 1:size(f, 1)
    new_y = sin(2*pi*cumsum(f(k,:)*ts));
    s = find(f(k,:) > 0, 1, 'first');
    e = find(f(k,:) > 0, 1, 'last');
    y = y + [zeros(1, s-1) fade(new_y(s:e)) zeros(1, T/ts-e+1)];
end

soundsc(y, fs)

% pause(2)
% clear sound

