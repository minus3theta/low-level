%ifndef _MACRO_
%define _MACRO_

%define head 0

%macro native 3
section .data
%%lw:
dq head
db %1, 0
db %3
xt_ %+ %2:
dq %2 %+ _impl
section .text
%2 %+ _impl:
%define head %%lw
%endmacro


%macro native 2
native %1, %2, 0
%endmacro


%endif
