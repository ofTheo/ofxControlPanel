#include "guiValue.h"

//------------------------------------------------
guiValue::guiValue(){

}

guiValue::~guiValue(){
    for(int i = 0; i < paramGroup.size(); i++){
        paramGroup.get(i).setParent(NULL);
    }
}

//------------------------------------------------
bool guiValue::addValue(ofAbstractParameter &param){
    if( param.getParent() ){
        paramGroup.setName( param.getParent()->getName() );
    }
    
    paramGroup.add(param);
    return true;
}

//------------------------------------------------
bool guiValue::setValue(float val, int which){
    string type = getTypeAsString(which);
    
    if( type == "float" || type == "" ){
        ofParameter<float> f =  paramGroup.getFloat(which);
        f = val;
        return true;
    }
    else if( type == "bool" ){
        ofParameter<bool> f =  paramGroup.getBool(which);
        f = val;
        return true;
    }
    else if( type == "int" ){
        ofParameter<int> f =  paramGroup.getInt(which);
        f = val;
        return true;
    }

    return false;
}

//------------------------------------------------
bool guiValue::setValueAsPct(float percent, int which){
    if( which >= paramGroup.size() ) return false;
    
    string type = getTypeAsString(which);

    if( type == "float" || type == "" ){
        ofParameter<float> f =  paramGroup.getFloat(which);
        f = f.getMin() + ofClamp(percent, 0, 1) * (f.getMax()-f.getMin());
        return true;
    }
    else if( type == "bool" ){
        ofParameter<bool> f =  paramGroup.getBool(which);
        if( percent == 0.0 ){
            f = false;
        }else{
            f = true;
        }
        return true;
    }
    else if( type == "int" ){
        ofParameter<int> f =  paramGroup.getInt(which);
        f = (float)f.getMin() + ofClamp(percent, 0, 1) * (float)(f.getMax()-f.getMin());
        return true;
    }
    
    return false;
}

//------------------------------------------------
string guiValue::getTypeAsString(int which){
    if( paramGroup.size() == 0 )return "";
    
    if( paramGroup.get(which).type() == typeid(ofParameter<float>).name() ){
        return "float";
    }
    else if( paramGroup.get(which).type() == typeid(ofParameter<int>).name() ){
        return "int";
    }
    else if( paramGroup.get(which).type() == typeid(ofParameter<bool>).name() ){
        return "bool";
    }
    else if( paramGroup.get(which).type() == typeid(ofParameter<string>).name() ){
        return "string";
    }
    
    return "";
}

//------------------------------------------------
float guiValue::getValueF(int which){

    string type = getTypeAsString(which);

    if( type == "float" || type == "" ){
        ofParameter<float> f =  paramGroup.getFloat(which);
        return f;
    }
    else if( type == "bool" ){
        ofParameter<bool> f =  paramGroup.getBool(which);
        return f;
    }
    else if( type == "int" ){
        ofParameter<int> f =  paramGroup.getInt(which);
        return f;
    }

    return 0.0;
}


//------------------------------------------------
string guiValue::getValueAsString(int which){
    return paramGroup.get(which).toString();
}

//------------------------------------------------
float guiValue::getValueI(int which){
    string type = getTypeAsString(which);

    if( type == "float" || type == "" ){
        ofParameter<float> f =  paramGroup.getFloat(which);
        return f;
    }
    else if( type == "bool" ){
        ofParameter<bool> f =  paramGroup.getBool(which);
        return f;
    }
    else if( type == "int" ){
        ofParameter<int> f =  paramGroup.getInt(which);
        return f;
    }

    return 0;
}

//------------------------------------------------
float guiValue::getValueB(int which){
    string type = getTypeAsString(which);

    if( type == "float" || type == "" ){
        ofParameter<float> f =  paramGroup.getFloat(which);
        return f;
    }
    else if( type == "bool" ){
        ofParameter<bool> f =  paramGroup.getBool(which);
        return f;
    }
    else if( type == "int" ){
        ofParameter<int> f =  paramGroup.getInt(which);
        return f;
    }

    return false;
}

//------------------------------------------------
float guiValue::getMin(int which){

    string type = getTypeAsString(which);

    if( type == "float" || type == "" ){
        ofParameter<float> f =  paramGroup.getFloat(which);
        return f.getMin();
    }
    else if( type == "bool" ){
        ofParameter<bool> f =  paramGroup.getBool(which);
        return f.getMin();
    }
    else if( type == "int" ){
        ofParameter<int> f =  paramGroup.getInt(which);
        return f.getMin();
    }

    return 0.0;

}

//------------------------------------------------
float guiValue::getMax(int which){

    string type = getTypeAsString(which);

    if( type == "float" || type == "" ){
        ofParameter<float> f =  paramGroup.getFloat(which);
        return f.getMax();
    }
    else if( type == "bool" ){
        ofParameter<bool> f =  paramGroup.getBool(which);
        return f.getMax();
    }
    else if( type == "int" ){
        ofParameter<int> f =  paramGroup.getInt(which);
        return f.getMax();
    }

    return 1.0;
}

//------------------------------------------------
void guiValue::setMin(float newMin, int which){

    string type = getTypeAsString(which);

    if( type == "float" || type == "" ){
        ofParameter<float> f =  paramGroup.getFloat(which);
        f.setMin(newMin);
    }
    else if( type == "bool" ){
        ofParameter<bool> f =  paramGroup.getBool(which);
        f.setMin(newMin);
    }
    else if( type == "int" ){
        ofParameter<int> f =  paramGroup.getInt(which);
        f.setMin(newMin);
    }
}

//------------------------------------------------						
void guiValue::setMax(float newMax, int which){

    string type = getTypeAsString(which);

    if( type == "float" || type == "" ){
        ofParameter<float> f =  paramGroup.getFloat(which);
        f.setMax(newMax);
    }
    else if( type == "bool" ){
        ofParameter<bool> f =  paramGroup.getBool(which);
        f.setMax(newMax);
    }
    else if( type == "int" ){
        ofParameter<int> f =  paramGroup.getInt(which);
        f.setMax(newMax);
    }
    
}

//------------------------------------------------
float guiValue::getPct(int which){
    if( which >= paramGroup.size() ) return 0.0;

    float min = 0.0;
    float max = 1.0;
    float val = 0.0;

    string type = getTypeAsString(which);

    if( type == "float" || type == "" ){
        ofParameter<float> f =  paramGroup.getFloat(which);
        min = f.getMin();
        max = f.getMax();
        val = f;
    }
    else if( type == "bool" ){
        ofParameter<bool> f =  paramGroup.getBool(which);
        val = f;
    }
    else if( type == "int" ){
        ofParameter<int> f =  paramGroup.getInt(which);
        min = f.getMin();
        max = f.getMax();
        val = f;
    }

    float delta = max-min;
    if(delta == 0.0){
        return 0.0;
    }
    
    float pct = (val-min) / delta;
    return ofClamp(pct, 0, 1);
}

//------------------------------------------------
int guiValue::getNumValues(){
    return paramGroup.size();
}



