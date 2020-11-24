

% extract_STC('de_Sqmdct.txt','n_msg_bits.txt','extr_秘密信息.txt')
function extr_STC (stego_name, n_msg_bits_name, extr_msg_name)
clc;
stego = load(stego_name);
stego = int32(stego);
n_msg_bits = load(n_msg_bits_name);
n_msg_bits = uint32(n_msg_bits);
h = 10;   % use STCs with constraint_height = 10. Try values between 7 and 12.

extr_msg = stc_ml_extract(stego, n_msg_bits, h);%提取

frr = fopen(extr_msg_name, 'w' ) ;       
fwrite(frr, extr_msg, 'ubit1') ;
fclose(frr);

fprintf('Message was extracted correctly.\n');
end