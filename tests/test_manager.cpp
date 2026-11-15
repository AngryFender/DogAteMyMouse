#include <matchengine.h>
#include <gmock/gmock.h>
#include "gtest/gtest.h"
#include "mock/mockconfig.h"
#include "mock/mockkeyboard.h"
#include "mock/mockmouse.h"
#include "mock/mockscreenrenderer.h"
#include "mock/mockstrategy.h"
#include "mock/mocktargetkeymanager.h"
#include "manager.h"

namespace {

	class ManagerFix : public testing::Test {

	protected:
		testing::NiceMock<MockConfig> mock_config;
		testing::NiceMock<MockKeyboard> mock_keyboard;
		testing::NiceMock<MockMouse> mock_mouse;
		testing::NiceMock<MockScreenRenderer> mock_screen_renderer;
		testing::NiceMock<MockScreenCapturer> mock_screen_capturer;
		testing::NiceMock<MockTargetKeyManager> mock_target_key_manager;
		testing::NiceMock<MockStrategy> mock_strategy;
	};

	TEST_F(ManagerFix, init) {

		EXPECT_CALL(mock_screen_renderer, init()).Times(0);

		Manager<
			MockConfig&,
			MockScreenRenderer&,
			MockKeyboard&,
			MockScreenCapturer&,
			MockMouse&,
			MockTargetKeyManager&,
			MockStrategy&
		>
			manager(
				mock_config,
				mock_screen_renderer,
				mock_keyboard,
				mock_screen_capturer,
				mock_mouse,
				mock_target_key_manager,
				mock_strategy
			);
	}

	TEST_F(ManagerFix, set_callbacks) {

		EXPECT_CALL(mock_keyboard, set_overlay_callback(testing::_, testing::_)).Times(1);
		EXPECT_CALL(mock_keyboard, set_keypress_callback(testing::_)).Times(1);
		EXPECT_CALL(mock_keyboard, set_overlay_state_getter(testing::_)).Times(1);

		Manager<
			MockConfig&,
			MockScreenRenderer&,
			MockKeyboard&,
			MockScreenCapturer&,
			MockMouse&,
			MockTargetKeyManager&,
			MockStrategy&
		>
			manager(
				mock_config,
				mock_screen_renderer,
				mock_keyboard,
				mock_screen_capturer,
				mock_mouse,
				mock_target_key_manager,
				mock_strategy
			);
	}

	TEST_F(ManagerFix, start_stop ) {

		EXPECT_CALL(mock_keyboard, set_overlay_callback(testing::_, testing::_)).Times(1);
		EXPECT_CALL(mock_keyboard, set_keypress_callback(testing::_)).Times(1);
		EXPECT_CALL(mock_keyboard, set_overlay_state_getter(testing::_)).Times(1);

		Manager<
			MockConfig&,
			MockScreenRenderer&,
			MockKeyboard&,
			MockScreenCapturer&,
			MockMouse&,
			MockTargetKeyManager&,
			MockStrategy&
		>
			manager(
				mock_config,
				mock_screen_renderer,
				mock_keyboard,
				mock_screen_capturer,
				mock_mouse,
				mock_target_key_manager,
				mock_strategy
			);
		manager.start();
		manager.stop();
	}
}
