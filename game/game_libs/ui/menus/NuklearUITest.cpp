#include "Framework.h"
#include "nuklear_impl/nuklear_ui.h"
#include "nuklear_impl/nuklear_renderer.h"

class CMenuNuklearUITest : public CMenuFramework
{
public:
	CMenuNuklearUITest() :
		CMenuFramework("CMenuNuklearUITest")
	{
	}

	~CMenuNuklearUITest() override
	{
		if ( m_CtxInitialised )
		{
			nk_free(&m_NkCtx);
			m_CtxInitialised = false;
		}
	}

	bool KeyDown(int key) override
	{
		bool b = CMenuFramework::KeyDown(key);
		RecordKeyEvent(m_KeysDownThisFrame, key);
		return b;
	}

	bool KeyUp(int key) override
	{
		bool b = CMenuFramework::KeyDown(key);
		RecordKeyEvent(m_KeysUpThisFrame, key);
		return b;
	}

	void Draw() override
	{
		CMenuFramework::Draw();

		if ( m_CtxInitialised )
		{
			static int op = 1;
			static float value = 0.6f;

			nk_input_begin(&m_NkCtx);

			if ( !uiStatic.hideCursor )
			{
				nk_input_motion(&m_NkCtx, uiStatic.cursorX, uiStatic.cursorY);
			}

			for ( size_t pass = 0; pass < 2; ++pass )
			{
				const bool keyDown = pass == 0;
				const int* events = keyDown ? m_KeysDownThisFrame : m_KeysUpThisFrame;

				for ( size_t index = 0; index < NUM_EVENTS; ++index )
				{
					int key = events[index];

					if ( key == 0 )
					{
						continue;
					}

					if ( UI::Key::IsLeftMouse(key) )
					{
						nk_input_button(&m_NkCtx, NK_BUTTON_LEFT, uiStatic.cursorX, uiStatic.cursorY, keyDown);
					}
					else if ( UI::Key::IsRightMouse(key) )
					{
						nk_input_button(&m_NkCtx, NK_BUTTON_RIGHT, uiStatic.cursorX, uiStatic.cursorY, keyDown);
					}
				}
			}

			nk_input_end(&m_NkCtx);

			if ( nk_begin(
					 &m_NkCtx,
					 "Show",
					 nk_rect(250, 50, 500, 500),
					 NK_WINDOW_BORDER | NK_WINDOW_MOVABLE | NK_WINDOW_CLOSABLE) )
			{
				// fixed widget pixel width
				nk_layout_row_static(&m_NkCtx, 30, 80, 1);

				if ( nk_button_label(&m_NkCtx, "button") )
				{
					value -= -0.1f;
				}

				// fixed widget window ratio width
				nk_layout_row_dynamic(&m_NkCtx, 30, 2);

				if ( nk_option_label(&m_NkCtx, "easy", op == 1) )
				{
					op = 1;
				}

				if ( nk_option_label(&m_NkCtx, "hard", op == 2) )
				{
					op = 2;
				}

				// custom widget pixel width
				nk_layout_row_begin(&m_NkCtx, NK_STATIC, 30, 2);
				{
					nk_layout_row_push(&m_NkCtx, 50);
					nk_label(&m_NkCtx, "Volume:", NK_TEXT_LEFT);
					nk_layout_row_push(&m_NkCtx, 110);
					nk_slider_float(&m_NkCtx, 0, &value, 1.0f, 0.1f);
				}

				nk_layout_row_end(&m_NkCtx);
			}

			nk_end(&m_NkCtx);

			const struct nk_command* cmd = 0;

			nk_foreach(cmd, &m_NkCtx)
			{
				Nuklear::HandleNuklearDrawCommand(cmd);
			}

			nk_clear(&m_NkCtx);
		}

		memset(m_KeysDownThisFrame, 0, sizeof(m_KeysDownThisFrame));
		memset(m_KeysUpThisFrame, 0, sizeof(m_KeysUpThisFrame));
	}

private:
	static constexpr size_t NUM_EVENTS = 4;

	virtual void _Init() override
	{
		banner.SetPicture("gfx/shell/head_blank");

		AddItem(background);
		AddItem(banner);

		AddButton(L("Back"), L("Return to main menu"), PC_DONE, VoidCb(&CMenuNuklearUITest::Hide));

		if ( !m_CtxInitialised )
		{
			m_NkFontCfg.width = [](nk_handle, float height, const char* text, int) -> float
			{
				return static_cast<float>(
					g_FontMgr->GetTextWideScaled(uiStatic.hConsoleFont, text, static_cast<int>(height)));
			};

			m_NkFontCfg.height = static_cast<float>(g_FontMgr->GetFontTall(uiStatic.hConsoleFont));
			m_NkFontCfg.userdata.ptr = nullptr;

			nk_init_default(&m_NkCtx, &m_NkFontCfg);
			m_CtxInitialised = true;
		}
	}

	void RecordKeyEvent(int array[NUM_EVENTS], int key)
	{
		for ( size_t index = 0; index < NUM_EVENTS; ++index )
		{
			if ( array[index] == 0 )
			{
				array[index] = key;
				break;
			}
		}
	}

	nk_context m_NkCtx {};
	nk_user_font m_NkFontCfg {};
	bool m_CtxInitialised = false;

	int m_KeysDownThisFrame[NUM_EVENTS] {};
	int m_KeysUpThisFrame[NUM_EVENTS] = {};
};

ADD_MENU(menu_nuklearuitest, CMenuNuklearUITest, UI_NuklearUITest_Menu);
