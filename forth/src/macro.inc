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
dq i_ %+ %2
section .text
i_ %+ %2:
%define head %%lw
%endmacro


%macro native 2
native %1, %2, 0
%endmacro


%macro colon 3
section .data
%%lw:
dq head
db %1, 0
db %3
xt_ %+ %2:
dq docol
%define head %%lw
%endmacro


%macro colon 2
colon %1, %2, 0
%endmacro

%endif
