
#define FROM_JSON_FIELD(obj,field)\
  if(j.contains(#field)){\
    obj.field = j[#field];\
  }else{\
    Utils::warning << "Missing field: " << #obj << "." << #field << std::endl;\
  }

#define TO_JSON_FIELD(obj,field)\
  j[#field] = obj.field;
