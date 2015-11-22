program DrawSphere;

uses GPHTOOL;

procedure SetPalette;external;
procedure DrawPixel(x,y:integer;c:byte);external;
{$L pal.obj }

procedure ShadeSphere (Kd, Ks: real;
	SpecIndex, XCentre, YCentre, Radius: integer);
const
     Ilight = 140;
     K = 70.0;
     IaKa = 0.2;
     Hx = 0.325085; Hy = 0.325058; Hz = 0.888074;
     dx = 110.0; dy = 110.0; dz = 110.0;
     Lx = 0.57735; Ly = 0.57735; Lz = 0.57735;
var
     Ig, x, y, z: integer;
     rsquare, xsquare, ysquare, zsquare, denom, xn, yn, zn, LdotN, NnH,
		dist, distfactor, ambientterm, diffuseterm, specularterm: real;

	procedure CalculateLNandNnH (x, y, z, SpecIndex: integer;
		xn, yn, zn: real; var LdotN, dist, NnH: real);
	var
		NH: real;
	begin
		LdotN := xn*Lx + yn*Ly + zn*Lz;
		if LdotN <= 0.0 then LdotN := 0.0
		else
		begin
			dist := sqrt ( sqr(dx-x) + sqr(dy-y) + sqr(dz-z) );
			NH := Hx*xn + Hy*yn + Hz*zn;
			NnH := exp( SpecIndex*ln (NH));
		end;
	end; {CalculateLNandNnH}

begin {ShadeSphere}
	rsquare := sqr(radius);
	for y := -radius to radius do
	begin
		ysquare := sqr(y);
		for x := -radius to radius do
		begin
			xsquare := sqr(x);
			if (xsquare + ysquare) <= rsquare then
			begin
				z := round( sqrt(rsquare-xsquare-ysquare) );
				zsquare := sqr(z);
				denom := sqrt (xsquare+ysquare+zsquare);
				xn := x/denom; yn := y/denom; zn := z/denom;
				CalculateLNandNnH (x,y,z,SpecIndex,xn,yn,zn,LdotN,dist,
					NnH);
				ambientterm := IaKa;
				if LdotN <= 0.0 then
				begin
					Ig := round (255*ambientterm);
				end
				else
				begin
					distfactor := Ilight/(dist + K);
					diffuseterm := distfactor * Kd * NnH;
					Ig := round (255* (ambientterm + diffuseterm+
						specularterm));
				end;
          		DrawPixel(XCentre+x, YCentre+y, Ig);
			end;
		end;
	end;
end; { ShadeSphere }

var x1,y1:integer;
var diff,spec:real;
begin
     SetGraphMode($13);
     SetPalette;

	y1:=0;
	diff := 0.25;
	repeat
		x1 := 0;
		spec := 1.0;
		repeat
			ShadeSphere(diff,spec,1,(x1*50)+25,(y1*50)+25,25);
			x1 := x1 + 1;
			spec := spec - 0.25;
		until x1 = 5;
		y1 := y1 + 1;
		diff := diff + 0.25;
	until y1 = 4;

end.