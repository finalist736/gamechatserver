#include "settings.h"

void initSettings()
{
    // main server configuration
    if (!_settings.contains("port"))
    {
        _settings.setValue("port", 57051);
    }
    if (!_settings.contains("fork"))
    {
        _settings.setValue("fork", false);
    }

    // mysql configuration
    if (!_settings.contains("mdbhost"))
    {
        _settings.setValue("mdbhost", "localhost");
    }
    if (!_settings.contains("mdbport"))
    {
        _settings.setValue("mdbport", 3306);
    }
    if (!_settings.contains("mdbname"))
    {
        _settings.setValue("mdbname", "gcs");
    }
    if (!_settings.contains("mdbuser"))
    {
        _settings.setValue("mdbuser", "root");
    }
    if (!_settings.contains("mdbpass"))
    {
        _settings.setValue("mdbpass", "masterkey");
    }
}
