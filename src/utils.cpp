#include "utils.h"

namespace Utils
{
	BYTE* SigScan(const char* moduleName, const char* signature, const char* mask)
	{
		auto module = GetModuleHandle(moduleName);
		if (!module)
		{
			return nullptr;
		}

		MODULEINFO mi = { 0 };
		if (!GetModuleInformation(GetCurrentProcess(), module, &mi, sizeof(mi)))
		{
			return nullptr;
		}

		return SigScan(mi.lpBaseOfDll, mi.SizeOfImage, signature, mask);
	}

	BYTE* SigScan(void* base, size_t size, const char* signature, const char* mask)
	{
		size_t sigLength = strlen(mask);

		char* cur = reinterpret_cast<char*>(base);
		char* end = cur + size - sigLength;

		for (; cur != end; cur++)
		{
			bool found = true;
			for (unsigned int i = 0; i < sigLength; i++)
			{
				if (cur[i] != signature[i] && mask[i] != '?')
				{
					found = false;
					break;
				}
			}
			if (found)
			{
				return reinterpret_cast<BYTE*>(cur);
			}
		}

		return nullptr;
	}

	void* GetRetAddrSpoofGadget(const char* moduleName)
	{
		auto module = GetModuleHandle(moduleName);
		if (!module)
		{
			return nullptr;
		}

		MODULEINFO mi = { 0 };
		if (!GetModuleInformation(GetCurrentProcess(), module, &mi, sizeof(mi)))
		{
			return nullptr;
		}

		const char* signature = "\xFF\x23";
		const char* mask = "xx";

		BYTE* cur = reinterpret_cast<BYTE*>(mi.lpBaseOfDll);
		BYTE* end = cur + mi.SizeOfImage - 2;

		size_t size;
		do
		{
			size = end - cur;

			cur = SigScan(cur, size, signature, mask);

			// Return nullptr if no match was found
			if (!cur)
			{
				return nullptr;
			}

			// If a pointer was returned, check it is in an read-execute region (.text segment)
			MEMORY_BASIC_INFORMATION mbi = { 0 };
			VirtualQuery(reinterpret_cast<void*>(cur), &mbi, sizeof(mbi));
			if (mbi.Protect == PAGE_EXECUTE_READ)
			{
				// If it is executable, this is a valid gadget
				return cur;
			}

			// If the pointer doesn't point to executable memory, scan again from the next byte
			cur++;
		} while (cur < end);

		return nullptr;
	}

}