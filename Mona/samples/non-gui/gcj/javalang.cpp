#define _Jv_InitClass __Jv_InitClass
#include <gcj/javaprims.h>
#include <gcj/cni.h>
#undef _Jv_InitClass
#include <java/lang/System.h>
#include <java/io/PrintStream.h>
#include <sms_gc.h>
#ifdef MONA
#include <monapi.h>
#else
#include <stdio.h>
#include <string.h>
#endif

jint _Jv_CreateJavaVM (void* vm_args) {
	sms_gc_register(&::java::lang::System::out);
	::java::lang::System::out = new ::java::io::PrintStream(NULL);
	return 0;
}

extern "C" jobject _Jv_AllocObject(jclass type, jint size) {
	void* result = sms_gc_malloc(size);
	memset(result, 0, size);
	*(void**)result = type->vtable;
	return (jobject)result;
}

extern "C" jobject _Jv_AllocObjectNoFinalizer(jclass type, jint size) {
	return _Jv_AllocObject(type, size);
}

extern "C" void _Jv_InitClass(jclass type) {
}

extern "C" void _Jv_RegisterClass(jclass type) {
}

// dummy for types
::java::lang::Class _Jv_voidClass, _Jv_booleanClass, _Jv_byteClass, _Jv_charClass, _Jv_shortClass, _Jv_intClass, _Jv_longClass, _Jv_floatClass, _Jv_doubleClass;

extern "C" jobject _Jv_NewPrimArray(jclass type, jsize length) {
	int size = 4;
	if (type == &_Jv_byteClass || type == &_Jv_booleanClass) {
		size = 1;
	} else if (type == &_Jv_charClass || type == &_Jv_shortClass) {
		size = 2;
	} else if (type == &_Jv_longClass || type == &_Jv_doubleClass) {
		size = 8;
	}
	int sz = sizeof(::java::lang::Object) + sizeof(jsize) + (length + 1) * size;
	jobject result = (jobject)sms_gc_malloc(sz);
	memset(result, 0, sz);
	*(jsize*)(result + 1) = length;
	return result;
}

extern "C" jobjectArray _Jv_NewObjectArray(jsize length, jclass klass, jobject init) {
	int sz = sizeof(::java::lang::Object) + sizeof(jsize) + (length + 1) * sizeof(jobject);
	jobjectArray result = (jobjectArray)sms_gc_malloc(sz);
	if (init == NULL) {
		memset(result, 0, sz);
	} else {
		jobject* p = elements(result);
		for (int i = 0; i < length; i++)
			p[i] = init;
	}
	*(jsize*)((jobject)result + 1) = length;
	return result;
}

extern "C" void _Jv_CheckArrayStore(jobjectArray array, jobject obj) {
}

extern "C" void _Jv_ThrowBadArrayIndex(int index) {
	printf("BadArrayIndexException: %d\n", index);
	*(int*)NULL = 0;
}