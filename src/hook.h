#include "SKSE/Trampoline.h"
#include "util.h"
#include "editorID.hpp"
#pragma warning(disable: 4100)


namespace hooks
{

	class util
	{
	private:


	public:
		static void install();

		template <class T>
		static std::vector<T *> get_all()
		{
			std::vector<T *> result;

			if (const auto dataHandler = RE::TESDataHandler::GetSingleton(); dataHandler)
			{
				for (const auto &form : dataHandler->GetFormArray<T>())
				{
					if (!form)
					{
						continue;
					}
					result.push_back(form);
				}
			}

			return result;
		}
	};
};

// auto a = RE::TESForm::LookupByEditorID<RE::BGSSoundDescriptorForm>("aaaUDDragonAISpell");
