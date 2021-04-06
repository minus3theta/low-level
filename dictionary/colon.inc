%define head 0

%macro colon 2
%2:
dq head
db %1, 0
%define head %2
%endmacro
