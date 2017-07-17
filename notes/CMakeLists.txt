include(UseLATEX)

set(QPGI_NOTES_FILENAME "notes")
add_latex_document(${QPGI_NOTES_FILENAME}.tex BIBFILES biblio.bib)

add_custom_target(open)
add_custom_command(TARGET open COMMAND open -a Skim ${QPGI_NOTES_FILENAME}.pdf)
