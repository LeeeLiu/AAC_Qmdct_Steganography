% EXAMPLE OF MULTI-LAYERED EMBEDDING - +-1 PAYLOAD LIMITED SENDER
% implemented using syndrome-trellis codes

% embed_STC('1-Tf-loss.xls', '������Ϣ.txt')
function emb_STC(loss_filename, msg_name)
clc;
x = load(loss_filename)  ;  % ���ı�����Ϊ����
x = x';
cover = int32(x(1,:));             %����QMDCT
n = size(x,2);
h = 10;   % use STCs with constraint_height = 10. Try values between 7 and 12.
costs = zeros(3, n, 'single'); % for each pixel, assign cost of being changed
costs([1 3],1:n) = x([2 3],1:n);    %ʧ�����

f_id = fopen(msg_name, "r") ; % �����û���д��message
[msg] = fread( f_id, 'ubit1');
msg = uint8(msg)';
fclose(f_id);

[d, stego, n_msg_bits, l] = stc_pm1_pls_embed(cover, costs, msg, h);%Ƕ��
dlmwrite('Sqmdct.txt', stego, 'delimiter',' ');

fid = fopen('n_msg_bits.txt','w');
fprintf (fid,'%d ',n_msg_bits);
fclose (fid);

fprintf('Message was embeded correctly.\n');
fprintf('achieved coding_loss = %4.2f%%\n', l*100);

end