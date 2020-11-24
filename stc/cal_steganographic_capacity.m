

% cal_steganographic_capacity('1-Tf-loss.xls', '512');
function cal_steganographic_capacity(loss_filename,reciprocal_payload)
clc;
x = load(loss_filename)  ;  % ¶ÁÎÄ±¾£¬´æÎª¾ØÕó
x = x';
n = size(x,2);  % m = round(n/reciprocal_payload);
m = n;  %¼òµ¥´Ö±©start
if (reciprocal_payload=='002')
   m = round(m/2);
end
if (reciprocal_payload=='004')
   m = round(m/4);
end
if (reciprocal_payload=='008')
   m = round(m/8);
end
if (reciprocal_payload=='016')
   m = (m/8);
   m = round(m/2);
end
if (reciprocal_payload=='032')
   m = (m/8);
   m = round(m/4);
end
if (reciprocal_payload=='064')
   m = (m/8);
   m = round(m/8);
end
if (reciprocal_payload=='128')
   m = (m/8);
   m = (m/8);
   m = round(m/2);
end
if (reciprocal_payload=='256')
   m = (m/8);
   m = (m/8);
   m = round(m/4);
end
if (reciprocal_payload=='512')
   m = (m/8);
   m = (m/8);
   m = round(m/8);
end %¼òµ¥´Ö±©end

fid = fopen('steganographic_capacity.txt','w');
fprintf (fid,'%d ',m);
fclose (fid);

end