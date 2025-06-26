#include "unpackers/common/CollectionsHolder.hh"

ClassImp(unpackers::common::CollectionsHolder);

using namespace unpackers::common;

CollectionsHolder::CollectionsHolder()
    : className_("CollectionsHolder")
{}

CollectionsHolder::~CollectionsHolder() {}

std::map<std::string,std::shared_ptr<data_products::common::DataProductPtrCollection>>
CollectionsHolder::GetCollections() {
    return basePtrCols_;
}

void CollectionsHolder::ClearCollections() {
    //loop over each collection and clear
    for (auto& col : basePtrCols_) { col.second->clear(); }
}

void CollectionsHolder::RegisterCollections(const std::map<std::string,std::shared_ptr<data_products::common::DataProductPtrCollection>>& colsToAdd) {
    //Register the provided collections
    for (const auto& colToAdd : colsToAdd) {
       this->RegisterCollection(colToAdd.first,colToAdd.second);
    }
}

void CollectionsHolder::RegisterCollection(const std::string label, const std::shared_ptr<data_products::common::DataProductPtrCollection> colPtr) {
    //find if label already exists
    auto it = basePtrCols_.find(label);
    if (it != basePtrCols_.end()) {
        std::cerr << "Error: a dataProduct with this label has already been registered\n"
        << "Details: label = " << label << std::endl;
        exit(1);
    } else {
        //Register the collection
        basePtrCols_[label] = colPtr;
    }
}
