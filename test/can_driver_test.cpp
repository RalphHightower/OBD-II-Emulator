#include <gtest/gtest.h>
#include <obdemulator/can_driver.h>

namespace ObdEmulator
{
    TEST(CanDriverTest, GetConfigurationMethod)
    {
        const CanBusSpeed cSpeed{CanBusSpeed::Speed5kbps};
        const bool cSupportExtended{true};

        const std::array<uint8_t, CanDriver::cFixedFrameSize> cExpectedResult{
            0xaa, 0x55, 0x12,
            0x0c,
            0x02,
            0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00,
            0x00,
            0x01, 0x00, 0x00, 0x00, 0x00,
            0x21};

        auto _actualResult{CanDriver::GetConfiguration(cSpeed, cSupportExtended)};

        EXPECT_TRUE(
            std::equal(
                cExpectedResult.begin(),
                cExpectedResult.end(),
                _actualResult.begin()));
    }

    TEST(CanDriverTest, StandardSerializeMethod)
    {
        const uint32_t cId{0x0123};
        const bool cExtended{false};
        const bool cRtr{false};
        const std::initializer_list<uint8_t> cData{
            0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88};

        const CanFrame cFrame(cId, cExtended, cRtr, cData);

        const std::array<uint8_t, CanDriver::cFixedFrameSize> cExpectedResult{
            0xaa, 0xc8,
            0x23, 0x01,
            0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88,
            0x55};

        auto _actualResult{CanDriver::Serialize(cFrame)};

        EXPECT_TRUE(
            std::equal(
                cExpectedResult.begin(),
                cExpectedResult.end(),
                _actualResult.begin()));
    }

    TEST(CanDriverTest, ExtendedSerializeMethod)
    {
        const uint32_t cId{0x01234567};
        const bool cExtended{true};
        const bool cRtr{false};
        const std::initializer_list<uint8_t> cData{
            0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88};

        const CanFrame cFrame(cId, cExtended, cRtr, cData);

        const std::array<uint8_t, CanDriver::cFixedFrameSize> cExpectedResult{
            0xaa, 0xe8,
            0x67, 0x45, 0x23, 0x01,
            0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88,
            0x55};

        auto _actualResult{CanDriver::Serialize(cFrame)};

        EXPECT_TRUE(
            std::equal(
                cExpectedResult.begin(),
                cExpectedResult.end(),
                _actualResult.begin()));
    }

    TEST(CanDriverTest, StandardDeserializeMethod)
    {
        const uint32_t cId{0x0123};
        const bool cExtended{false};
        const bool cRtr{false};
        const std::initializer_list<uint8_t> cData{
            0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88};

        const std::initializer_list<uint8_t> cPacket{
            0xaa, 0xc8,
            0x23, 0x01,
            0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88,
            0x55};

        CanFrame _frame{CanDriver::Deserialize(cPacket)};

        EXPECT_EQ(_frame.GetId(), cId);
        EXPECT_EQ(_frame.IsExtended(), cExtended);
        EXPECT_EQ(_frame.IsRemote(), cRtr);
        EXPECT_EQ(_frame.GetDataLength(), cData.size());
        EXPECT_TRUE(
            std::equal(
                cData.begin(),
                cData.end(),
                _frame.GetData().begin()));
    }

    TEST(CanDriverTest, ExtendedDeserializeMethod)
    {
        const uint32_t cId{0x01234567};
        const bool cExtended{true};
        const bool cRtr{false};
        const std::initializer_list<uint8_t> cData{
            0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88};

        const std::initializer_list<uint8_t> cPacket{
            0xaa, 0xe8,
            0x67, 0x45, 0x23, 0x01,
            0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88,
            0x55};

        CanFrame _frame{CanDriver::Deserialize(cPacket)};

        EXPECT_EQ(_frame.GetId(), cId);
        EXPECT_EQ(_frame.IsExtended(), cExtended);
        EXPECT_EQ(_frame.IsRemote(), cRtr);
        EXPECT_EQ(_frame.GetDataLength(), cData.size());
        EXPECT_TRUE(
            std::equal(
                cData.begin(),
                cData.end(),
                _frame.GetData().begin()));
    }
}