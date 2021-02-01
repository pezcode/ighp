if exist x64\Release\GlobalHotkeys.dll (
    makensis /DIS64BIT ighp.nsi
)

if exist Release\GlobalHotkeys.dll (
    makensis ighp.nsi
)
