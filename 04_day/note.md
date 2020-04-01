# リンカスクリプトの修正
- コンパイルしようとすると
- section .note.gnu.property LMA [00000000000001a4,00000000000001bf] overlaps section .data LMA [00000000000001a4,00000000000001d3]
- といわれてしまうので、har.ldで note.gnu.propertyをDISCARDする

# CLI,STI
- CLIは割込フラグをclear,STIは割込フラグはstore