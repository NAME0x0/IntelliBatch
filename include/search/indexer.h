#pragma once

#include <wx/wx.h>
#include <vector>
#include <string>
#include <unordered_map>
#include <thread>
#include <mutex>
#include <atomic>
#include <set>
#include <memory>

namespace ITD {

/**
 * @brief File information structure
 */
struct FileInfo {
    wxString path;             ///< Full file path
    wxString name;             ///< File name
    wxString extension;        ///< File extension
    wxDateTime lastModified;   ///< Last modified time
    bool isDirectory;          ///< True if this is a directory
    uint64_t size;             ///< File size in bytes

    /**
     * @brief Constructor
     */
    FileInfo() : isDirectory(false), size(0) {}

    /**
     * @brief Constructor with path
     * @param filePath File path
     */
    explicit FileInfo(const wxString& filePath);
};

/**
 * @brief Search result structure
 */
struct SearchResult {
    FileInfo fileInfo;         ///< File information
    double score;              ///< Relevance score (higher is better)
    std::vector<wxString> matchingTerms;  ///< Terms that matched

    /**
     * @brief Constructor
     */
    SearchResult() : score(0.0) {}
};

/**
 * @brief File indexing and search component
 * 
 * This class provides fast file indexing and searching capabilities,
 * similar to Everything or Find tools.
 */
class Indexer : public wxEvtHandler {
public:
    /**
     * @brief Constructor
     */
    Indexer();
    
    /**
     * @brief Destructor
     */
    virtual ~Indexer();

    /**
     * @brief Start indexing a directory
     * @param directory Directory path to index
     * @param recursive True to index subdirectories recursively
     * @return True if indexing started successfully
     */
    bool StartIndexing(const wxString& directory, bool recursive = true);

    /**
     * @brief Stop current indexing operation
     */
    void StopIndexing();

    /**
     * @brief Check if indexing is in progress
     * @return True if indexing is in progress
     */
    bool IsIndexing() const { return m_isIndexing; }

    /**
     * @brief Get indexing progress
     * @return Progress as a percentage (0-100)
     */
    int GetIndexingProgress() const { return m_indexingProgress; }

    /**
     * @brief Set directories to exclude from indexing
     * @param excludeDirs List of directory paths to exclude
     */
    void SetExcludeDirectories(const std::vector<wxString>& excludeDirs);

    /**
     * @brief Set file patterns to exclude from indexing
     * @param excludePatterns List of file patterns to exclude (e.g., "*.tmp")
     */
    void SetExcludePatterns(const std::vector<wxString>& excludePatterns);

    /**
     * @brief Search for files
     * @param query Search query
     * @param maxResults Maximum number of results to return
     * @return List of search results
     */
    std::vector<SearchResult> Search(const wxString& query, size_t maxResults = 100);

    /**
     * @brief Get indexed directories
     * @return List of indexed directory paths
     */
    std::vector<wxString> GetIndexedDirectories() const;

    /**
     * @brief Get total indexed file count
     * @return Number of indexed files
     */
    size_t GetTotalIndexedFiles() const { return m_files.size(); }

    /**
     * @brief Save index to disk
     * @param filename File path to save to
     * @return True if successful
     */
    bool SaveIndex(const wxString& filename);

    /**
     * @brief Load index from disk
     * @param filename File path to load from
     * @return True if successful
     */
    bool LoadIndex(const wxString& filename);

    /**
     * @brief Clear the index
     */
    void ClearIndex();

private:
    std::unordered_map<wxString, FileInfo> m_files;  ///< Indexed files
    std::unordered_map<wxString, std::set<wxString>> m_terms;  ///< Search terms to file paths

    std::atomic<bool> m_isIndexing;     ///< Indexing status
    std::atomic<int> m_indexingProgress;  ///< Indexing progress (0-100)
    std::unique_ptr<std::thread> m_indexingThread;  ///< Indexing thread
    std::mutex m_indexMutex;            ///< Mutex for thread safety

    std::vector<wxString> m_excludeDirectories;  ///< Directories to exclude
    std::vector<wxString> m_excludePatterns;     ///< File patterns to exclude
    std::set<wxString> m_indexedDirectories;     ///< Directories that have been indexed

    // Indexing thread function
    void IndexDirectory(const wxString& directory, bool recursive);

    // Check if a file should be excluded
    bool ShouldExclude(const wxString& path);

    // Extract search terms from a file path
    std::vector<wxString> ExtractSearchTerms(const wxString& path);

    // Calculate relevance score for a file
    double CalculateScore(const FileInfo& fileInfo, const std::vector<wxString>& queryTerms);

    // Fire events for index status updates
    void FireIndexingUpdateEvent();
    void FireIndexingFinishedEvent();
};

// Custom event types
wxDECLARE_EVENT(EVT_INDEXING_PROGRESS, wxCommandEvent);
wxDECLARE_EVENT(EVT_INDEXING_FINISHED, wxCommandEvent);

} // namespace ITD 