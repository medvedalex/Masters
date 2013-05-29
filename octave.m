data= [
0.648544439097;
0.464020074781;
0.400421829034;
0.389424787208;
0.356103771131;
0.333437650995;
0.319533990268;
0.300862734095;
0.289915765805;
0.284321585423;
];

leasqrfunc = @(x, p) p(1) * exp (-p(2) * x) +p(3) * exp (-p(4) * x)+ p(5) * exp (-p(6) * x)  ;
function retval =  leasqrfuncMod (x,p)
 	
	if  (leasqrfunc(x, p) > data(1+x) )
		retval =   (leasqrfunc(x, p)./data(1+x)); 
		return;
	else
		retval =  data(1+x)./(leasqrfunc(x, p)); 
		return;
	endif
endfunction
  
function retval =  leasqrdfdp (x,f,p,dp, func)
 	
if  (leasqrfunc(x, p) > data(1+x) )
	retval =   [  
		exp(-p(2)*x)./data(1+x),  
		-p(1)*x.*exp(-p(2)*x)./data(1+x),   
		exp(-p(4)*x)./data(1+x),   
		-p(3)*x.*exp(-p(4)*x)./data(1+x) ,   
		exp(-p(6)*x)./data(1+x),   
		-p(5)*x.*exp(-p(6)*x)./data(1+x)
	];
	return;
else
	retval = [  
		-exp(-p(2).*x).*data(1+x)./(leasqrfunc(x, p).^2),  
		p(1)*x.*exp(-p(2).*x).*data(1+x)./(leasqrfunc(x, p).^2),  
		-exp(-p(4).*x).*data(1+x)./(leasqrfunc(x, p).^2), 
		p(3)*x.*exp(-p(4).*x).*data(1+x)./(leasqrfunc(x, p).^2),  
		-exp(-p(6).*x).*data(1+x)./(leasqrfunc(x,p).^2),  
		p(5)*x.*exp(-p(6).*x).*data(1+x)./(leasqrfunc(x, p).^2) 
	];
	return;
endif
endfunction

t = [0;1;2;3;4;5;6;7;8;9];

data_const = [1;1;1;1;1;1;1;1;1;1;];
wt = (1 + 0 * t) ./ sqrt (data); 
F = @leasqrfuncMod;
dFdp = @leasqrdfdp; 
dp = [0.0001; 0.0001;0.0001;0.0001 ;0.0001;0.0001];
pin = [
0.4;0.0408677143846;0.15;2.23130160148;0.1;0.497870683679; 
];
minstep = [0.0001; 0.0001;0.0001; 0.0001;0.0001; 0.0001];
maxstep = [0.8; 0.8;0.8; 0.8;0.8; 0.8];
options = [minstep, maxstep];
[f1, p1, kvg1, iter1, corp1, covp1, covr1, stdresid1, Z1, r21] =  leasqr (t, data_const, pin, F, stol, niter, wt, dp, dFdp, options);
 sum(((leasqrfunc(t, p1)-data)./min(data, leasqrfunc(t, p1))*100).^2)./10
(data)./min(data, leasqrfunc(t, p1))
