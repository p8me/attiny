
fs = 48000;
ts = 1/fs;
sp = 240;
T = 12;

fade_win = 0.02;

arc = @(r, t) r * sin(acos(((ts:ts:t)/t-0.5)*2));
circ = @(r, t) [1; -1] * arc(r, t);
linD = @(deg, t) (ts:ts:t) * sp * tan(pi/180 * deg);
linP = @(y1, y2, t) y1 + (ts:ts:t) * (y2-y1)/t;
fit = @(s, T, f) [nan(size(f, 1), round(s/ts)), f, nan(size(f, 1), round((T - s)/ts) - size(f, 2))];

I = @(t, H) [[1; -1] * H/2 + linD(0, t); fit(t/2-t/20, t, linP(-H/2, H/2, t/10))];
L = @(t, H) [fit(0, t, linP(-H/2, H/2, t/10)); - H/2 + linD(0, t)];
O = @(t, H) circ(H/2, t);
V = @(t, H) [fit(0, t, linP(H/2, -H/2, t/2)); fit(t/2, t, linP(-H/2, H/2, t/2))];
E = @(t, H) [fit(0, t, linP(-H/2, H/2, t/10)); [1; 0; -1] * H/2 + linD(0, t)];
U = @(t, H) [fit(0, t, linP(H/2, 0, t/20)); fit(t-t/20, t, linP(0, H/2, t/20)); fit(t/20, t, -arc(H/2, t-t/10))];
Heart = @(t) (t/7 + [sqrt(1 - (abs((-1:(ts*2/t):1)*2) - 1).^2)/2; .6 * (acos(2*.7 - 1 - abs((-1:(ts*2/t):1)*2)*.7) - pi)]) * t/2 * sp;

f = 1500 + [...
    800 + fit(4, T, circ(360, 3))
    800 + fit(4.5, T, Heart(2))
    20 + fit(7.5, T, Heart(1.5))
    fit(2.0, T, I(.7, 360))
    fit(3.0, T, L(.8, 360))
    fit(3.6, T, O(1, 360))
    fit(4.6, T, V(.8, 360))
    fit(5.5, T, E(.8, 360))
    fit(6.5, T, U(.8, 360))
    ];

f = f + (1:length(f)) * .0000;

% Simulate
figure(1)
plot(ts:ts:T, f(1,:)'); axis equal
set(gca, "DataAspectRatio", [sp*ts*.85 1 1])
return;
%%
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

