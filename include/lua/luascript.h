#pragma once

#include <wx/wx.h>
#include <string>
#include <functional>
#include <unordered_map>
#include <vector>
#include <memory>

// Forward declaration for Lua state
struct lua_State;

namespace ITD {

/**
 * @brief Lua scripting engine for ITD
 * 
 * This class provides Lua scripting capabilities for the application,
 * allowing users to customize behavior and extend functionality.
 */
class LuaScript {
public:
    /**
     * @brief Constructor
     */
    LuaScript();
    
    /**
     * @brief Destructor
     */
    virtual ~LuaScript();

    /**
     * @brief Load and execute a Lua script file
     * @param filename Path to the script file
     * @return True if successful, false otherwise
     */
    bool LoadFile(const wxString& filename);

    /**
     * @brief Execute a Lua script string
     * @param script Lua script code
     * @return True if successful, false otherwise
     */
    bool Execute(const wxString& script);

    /**
     * @brief Call a Lua function
     * @param functionName Name of the function to call
     * @param args Arguments to pass to the function
     * @return True if successful, false otherwise
     */
    template<typename... Args>
    bool CallFunction(const std::string& functionName, Args... args);

    /**
     * @brief Register a C++ function to be callable from Lua
     * @param name Function name in Lua
     * @param function C++ function to register
     */
    template<typename Ret, typename... Args>
    void RegisterFunction(const std::string& name, Ret(*function)(Args...));

    /**
     * @brief Check if a Lua function exists
     * @param functionName Name of the function to check
     * @return True if the function exists, false otherwise
     */
    bool FunctionExists(const std::string& functionName);

    /**
     * @brief Get the last error message
     * @return Last error message
     */
    const wxString& GetLastError() const { return m_lastError; }

    /**
     * @brief Add a directory to the Lua path
     * @param path Directory path to add
     */
    void AddLuaPath(const wxString& path);

    /**
     * @brief Get the list of loaded script files
     * @return Vector of loaded script paths
     */
    const std::vector<wxString>& GetLoadedScripts() const { return m_loadedScripts; }

private:
    lua_State* m_luaState = nullptr;  ///< Lua state
    wxString m_lastError;             ///< Last error message
    std::vector<wxString> m_loadedScripts;  ///< List of loaded script files

    // Register standard library functions
    void RegisterStandardFunctions();

    // Error handling
    void HandleLuaError();
};

// Application-specific API functions exposed to Lua
namespace LuaAPI {
    // Terminal functions
    int ExecuteCommand(lua_State* L);
    int GetCurrentDirectory(lua_State* L);
    
    // Widget functions
    int CreateWidget(lua_State* L);
    int RemoveWidget(lua_State* L);
    
    // Window management functions
    int SetLayoutType(lua_State* L);
    int FocusWindow(lua_State* L);
    int SplitWindow(lua_State* L);
    
    // UI functions
    int SetTransparency(lua_State* L);
    int ShowMessage(lua_State* L);
    int PromptInput(lua_State* L);
    
    // File system functions
    int ListDirectory(lua_State* L);
    int FileExists(lua_State* L);
    int ReadFile(lua_State* L);
    int WriteFile(lua_State* L);
    
    // Key binding functions
    int RegisterKeyBinding(lua_State* L);
    int UnregisterKeyBinding(lua_State* L);
}

} // namespace ITD 