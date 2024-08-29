#define TEST_EMBEDDED

#include "unity.h"
#include "Example_Unit_Test_Native.cpp"


void test_ui8_CheckArrayLenght()
{
    
    tUInt8 aui8_Data[100] = {0};
    tUInt8* p_aui8_Data = &aui8_Data[0];
    TEST_ASSERT_TRUE(b_SendMessage(p_aui8_Data, sizeof(aui8_Data)));

    p_aui8_Data = NULL;
    TEST_ASSERT_FALSE(b_SendMessage(p_aui8_Data, sizeof(aui8_Data)));
}

void test_v_ReadAcknowledge()
{
    string str = "82.00.12.24";
    tUInt8 aui8_DestArray[4];
    tUInt8 ui8_ExpectedArray[4] = {82, 0, 12, 24};
    v_ReadAcknowledge(str, sizeof(aui8_DestArray), &aui8_DestArray[0]);
    TEST_ASSERT_EQUAL_UINT8_ARRAY(ui8_ExpectedArray, aui8_DestArray, sizeof(aui8_DestArray));

    string str_0 = "1.2.3.4.5.6.0.257.2.34.5";
    tUInt8 aui8_DestArray_0[11];
    tUInt8 ui8_ExpectedArray_0[11] = {1,2,3,4,5,6,0,1,2,34,5};
    v_ReadAcknowledge(str_0, sizeof(aui8_DestArray_0), &aui8_DestArray_0[0]);
    TEST_ASSERT_EQUAL_UINT8_ARRAY(ui8_ExpectedArray_0, aui8_DestArray_0, sizeof(aui8_DestArray_0));


}

void setUp()
{

}

void tearDown()
{

}


int main(int argc, char **argv)
{
    UNITY_BEGIN();
    RUN_TEST(test_ui8_CheckArrayLenght);
    RUN_TEST(test_v_ReadAcknowledge);
    UNITY_END();
}
