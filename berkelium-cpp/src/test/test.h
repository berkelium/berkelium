#define ASSERT_NULL(ptr) ASSERT_EQ((void*)NULL, (void*)(ptr.get()))
#define ASSERT_NOT_NULL(ptr) ASSERT_NE((void*)NULL, (void*)(ptr.get()))
