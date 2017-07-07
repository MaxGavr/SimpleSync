#pragma once



class FileProperties
{
public:
    FileProperties(const CString& fileName = CString());
    ~FileProperties();

    BOOL operator< (const FileProperties& file) const;
    BOOL operator== (const FileProperties& file) const;

    CString getFileName() const;

private:
    CString m_fileName;
};

