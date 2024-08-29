from datetime import datetime


def writeComments(ProjectName, unit, fileName, author_firstName, author_lastName, functionNameList: list):

    extension : str = "cpp" 
    className : str = ""
    d1 = datetime.now()
    dt_string = d1.strftime("%d/%m/%Y %H:%M:%S")
    directoryPath = "C:/MakerFactoryLab/sw_makefactorylab/10_SRC/" + ProjectName + "/" +  unit + "/" + fileName
    author = author_lastName + ", " + author_firstName + " <"+ author_firstName.lower() + "." + author_lastName.lower() + "@dta-alliance.com>"

    if(unit != "src"):
        extension = "h"
    directoryPath += "." +extension

    comments = "\n\t/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n\
    @Author:\n\
    @Create Date: " + d1.strftime("%d/%m/%Y") + "\n\
    ---------------------------------------------------------------------\n\
    @Function description: \n\
    ---------------------------------------------------------------------\n\
    @Parameter: \n\
    @Return value: \n\
    +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/"

    head = "/**\n\
*@Copyright DTTA Maker Factory Tech. All Rights reserved.\n\
*It is not allowed, whether to copy this document, or to pass it even in extracts to others, without the approval DTTA Maker Factory Tech.\n\
*All copyrights for this document are the exclusive property of DTTA Maker Factory Tech.\n\
*/ \n\
/*----------------------------------------------------------------------*/\n\
#ifndef " + fileName.upper() + "_"+ extension.upper() + "\n\
#define " + fileName.upper() + "_"+ extension.upper() + "\n\
/**\n\
*@file: "+ fileName + "." + extension + "\n\
*@author: "+ author + " \n\
*@date: "+ dt_string + "\n */\
/*----------------------------------------------------------------------*/\n \n\
/*-----------------------------includes---------------------------------*/\n\
/* C system headers */ \n\
/* C++ standard library headers */ \n\
/* External libraires headers */ \n\
/* Arduino libraries headers */ \n \n \n\
/*-------------------------global defines-------------------------------*/\n\n" 

    classDeclaration = "class " + fileName + "\n\
{ \n\nprivate: \n\n\
public :\n" + comments + "\n\
\t" + fileName + "() \n\
\t:\n\
\t{\n\n\
\t}\n" + comments + "\n\
\t~" + fileName + "() \n\
\t{\n\n\
\t}\n"

    

    endFile = "\n};\n\
#endif //" + fileName.upper() + "_H"

    if extension != "h" :
        classDeclaration = ""

    file = open(directoryPath, "w")
    file.write(head)
    file.write(classDeclaration)
    functionDef = "\n\t"
    for i in range(len(functionNameList)) :
        #print(functionNameList[i][1:])
        functionDef += "".join(functionNameList[i][1:]) +" "+ " ".join(functionNameList[i][0:1]) + "( )\n\
    {\n\n\t}\n"
        
        file.write(comments)
        file.write(functionDef)
        functionDef = "\n\t"
    file.write(endFile)
    file.close()

functionNameList = [("ui8_ReadSoilMoistureValue", "tUInt8"),("b_OnEntry", "tBool"),
                    ("b_OnDuring", "tBool")]

writeComments("Soil/SoilMoisture", "include", "SoilMoistureMeasurement", "Cezy", "Emvoutou", functionNameList)