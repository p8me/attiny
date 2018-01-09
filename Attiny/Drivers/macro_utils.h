
#ifndef MACRO_UTILS_H
#define MACRO_UTILS_H

// FROM: http://stackoverflow.com/a/1872506/1698972
#define FE_1(ACT, X) ACT(X)
#define FE_2(ACT, X, ...)	ACT(X)FE_1(ACT, __VA_ARGS__)
#define FE_3(ACT, X, ...)	ACT(X)FE_2(ACT, __VA_ARGS__)
#define FE_4(ACT, X, ...)	ACT(X)FE_3(ACT, __VA_ARGS__)
#define FE_5(ACT, X, ...)	ACT(X)FE_4(ACT, __VA_ARGS__)
#define FE_6(ACT, X, ...)	ACT(X)FE_5(ACT, __VA_ARGS__)
#define FE_7(ACT, X, ...)	ACT(X)FE_6(ACT, __VA_ARGS__)
#define FE_8(ACT, X, ...)	ACT(X)FE_7(ACT, __VA_ARGS__)
#define FE_9(ACT, X, ...)	ACT(X)FE_8(ACT, __VA_ARGS__)
#define FE_10(ACT, X, ...)	ACT(X)FE_9(ACT, __VA_ARGS__)
#define FE_11(ACT, X, ...)	ACT(X)FE_10(ACT, __VA_ARGS__)
#define FE_12(ACT, X, ...)	ACT(X)FE_11(ACT, __VA_ARGS__)
#define FE_13(ACT, X, ...)	ACT(X)FE_12(ACT, __VA_ARGS__)
#define FE_14(ACT, X, ...)	ACT(X)FE_13(ACT, __VA_ARGS__)
#define FE_15(ACT, X, ...)	ACT(X)FE_14(ACT, __VA_ARGS__)
#define FE_16(ACT, X, ...)	ACT(X)FE_15(ACT, __VA_ARGS__)
#define FE_17(ACT, X, ...)	ACT(X)FE_16(ACT, __VA_ARGS__)
#define FE_18(ACT, X, ...)	ACT(X)FE_17(ACT, __VA_ARGS__)
#define FE_19(ACT, X, ...)	ACT(X)FE_18(ACT, __VA_ARGS__)
#define FE_20(ACT, X, ...)	ACT(X)FE_19(ACT, __VA_ARGS__)

#define GET_MACRO(_1,_2,_3,_4,_5,_6,_7,_8,_9,_10,_11,_12,_13,_14,_15,_16,_17,_18,_19,_20,NAME,...) NAME
#define FOR_EACH(ACT,...) GET_MACRO(__VA_ARGS__,FE_20,FE_19,FE_18,FE_17,FE_16,FE_15,FE_14,FE_13,FE_12,FE_11,FE_10,FE_9,FE_8,FE_7,FE_6,FE_5,FE_4,FE_3,FE_2,FE_1,)(ACT,__VA_ARGS__)

#define STRING(x...) #x
#define STRING2(x) STRING(x)

#endif
