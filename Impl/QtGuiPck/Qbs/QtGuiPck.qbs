import "../../../Config/Qbs/ComponentProduct.qbs" as ComponentProduct

ComponentProduct{
	name: "QtGuiPck"

	Depends{ name: "icomp" }
	Depends{ name: "iqtgui" }
	Depends{ name: "ifilegui" }
	Depends{ name: "iqtprm" }
	Depends{ name: "iqtdoc" }
	Depends{ name: "iloggui" }
}
