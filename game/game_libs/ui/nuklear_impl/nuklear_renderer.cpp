#include <utility>
#include <functional>
#include "nuklear_impl/nuklear_renderer.h"
#include "BuildPlatform/Utils.h"
#include "BaseMenu.h"

namespace Nuklear
{
	static void DrawRect(const struct nk_command* command)
	{
		const struct nk_command_rect* rect = reinterpret_cast<const struct nk_command_rect*>(command);

		UI_DrawRectangle(
			static_cast<int>(rect->x),
			static_cast<int>(rect->y),
			static_cast<int>(rect->w),
			static_cast<int>(rect->h),
			nk_color_u32(rect->color),
			static_cast<int>(rect->line_thickness));
	}

	static void DrawFilledRect(const struct nk_command* command)
	{
		const struct nk_command_rect_filled* rect = reinterpret_cast<const struct nk_command_rect_filled*>(command);

		UI_FillRect(
			static_cast<int>(rect->x),
			static_cast<int>(rect->y),
			static_cast<int>(rect->w),
			static_cast<int>(rect->h),
			nk_color_u32(rect->color));
	}

	static void DrawText(const struct nk_command* command)
	{
		const struct nk_command_text* text = reinterpret_cast<const struct nk_command_text*>(command);

		UI_DrawString(
			uiStatic.hConsoleFont,
			text->x,
			text->y,
			text->w, text->h,
			text->string,
			nk_color_u32(text->foreground),
			text->font->height,
			QM_TOPLEFT);
	}

	void HandleNuklearDrawCommand(const struct nk_command* command)
	{
		static const std::pair<enum nk_command_type, void (*)(const struct nk_command*)> HANDLERS[] = {
			{NK_COMMAND_RECT, DrawRect},
			{NK_COMMAND_RECT_FILLED, DrawFilledRect},
			{NK_COMMAND_TEXT, DrawText},
		};

		if ( !command )
		{
			return;
		}

		for ( size_t index = 0; index < SIZE_OF_ARRAY(HANDLERS); ++index )
		{
			if ( command->type == HANDLERS[index].first )
			{
				HANDLERS[index].second(command);
				break;
			}
		}
	}
}  // namespace Nuklear
