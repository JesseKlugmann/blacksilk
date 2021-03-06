#include <libgraphics/filterstack.hpp>
#include <QDebug>

namespace libgraphics {

size_t  FilterStack::count() const {
    return this->m_Filters.size();
}

std::shared_ptr<Filter>    FilterStack::top() {
    return this->m_Filters.front();
}

const std::shared_ptr<Filter>&    FilterStack::top() const {
    return this->m_Filters.front();
}

std::shared_ptr<Filter>    FilterStack::bottom() {
    return this->m_Filters.back();
}

const std::shared_ptr<Filter>&    FilterStack::bottom() const {
    return this->m_Filters.back();
}

void FilterStack::clear() {
    this->m_Filters.clear();
}

void FilterStack::pushFront( Filter* filter ) {
    this->m_Filters.insert(
        this->m_Filters.begin(),
        std::shared_ptr<Filter>( filter )
    );
}

void FilterStack::pushFront( const std::shared_ptr<Filter>& filter ) {
    this->m_Filters.insert(
        this->m_Filters.begin(),
        filter
    );
}

void FilterStack::pushBack( Filter* filter ) {
    this->m_Filters.push_back(
        std::shared_ptr<Filter>( filter )
    );
}

void FilterStack::pushBack( const std::shared_ptr<Filter>& filter ) {
    this->m_Filters.push_back(
        filter
    );
}

void FilterStack::insert( ContainerType::iterator it, Filter* filter ) {
    this->m_Filters.insert(
        it,
        std::shared_ptr<Filter>( filter )
    );
}

void FilterStack::insert( ContainerType::iterator it, const std::shared_ptr<Filter>& filter ) {
    this->m_Filters.insert(
        it,
        filter
    );
}

FilterStack::ContainerType::iterator FilterStack::begin() {
    return this->m_Filters.begin();
}

FilterStack::ContainerType::const_iterator FilterStack::begin() const {
    return this->m_Filters.begin();
}

FilterStack::ContainerType::const_iterator FilterStack::constBegin() const {
    return this->m_Filters.begin();
}

FilterStack::ContainerType::iterator FilterStack::end() {
    return this->m_Filters.end();
}

FilterStack::ContainerType::const_iterator FilterStack::end() const {
    return this->m_Filters.end();
}

FilterStack::ContainerType::const_iterator FilterStack::constEnd() const {
    return this->m_Filters.end();
}

std::shared_ptr<Filter>    FilterStack::byIndex( size_t index ) const {
    assert( this->m_Filters.size() > index );
    return *( this->m_Filters.begin() + index );
}

std::shared_ptr<Filter>    FilterStack::byName( const std::string& name ) const {
    for( auto it = this->m_Filters.begin(); it != this->m_Filters.end(); ++it ) {
        if( ( *it )->name() == name ) {
            return ( *it );
        }
    }

    return {};
}

bool FilterStack::remove( ContainerType::iterator it ) {
    this->m_Filters.erase(
        it
    );
    return true;
}

bool FilterStack::remove( const std::shared_ptr<Filter>& filter ) {
    for( auto it = this->m_Filters.begin(); it != this->m_Filters.end(); ++it ) {
        if( ( *it ) == filter ) {
            this->m_Filters.erase(
                it
            );
            return true;
        }
    }

    return false;
}

bool FilterStack::remove( Filter* filter ) {
    for( auto it = this->m_Filters.begin(); it != this->m_Filters.end(); ++it ) {
        if( it->get() == filter ) {
            this->m_Filters.erase(
                it
            );
            return true;
        }
    }

    return false;
}


bool FilterStack::removeByName( const std::string& name ) {
    for( auto it = this->m_Filters.begin(); it != this->m_Filters.end(); ++it ) {
        if( ( *it )->name() == name ) {
            this->m_Filters.erase(
                it
            );
            return true;
        }
    }

    return false;
}

bool FilterStack::removeByIndex( size_t index ) {
    assert( m_Filters.size() > index );

    if( this->m_Filters.size() > index ) {
        this->m_Filters.erase(
            this->m_Filters.begin() + index
        );
        return true;
    }

    return false;
}

bool FilterStack::process(
    libgraphics::ImageLayer*    destination,
    libgraphics::ImageLayer*    source
) {
    assert( destination );
    assert( source );

    for( auto it = this->m_Filters.begin(); it != this->m_Filters.end(); ++it ) {
        auto filter = ( *it );

        if( filter->backend() == nullptr ) {
#ifdef LIBGRAPHICS_DEBUG_OUTPUT
            qDebug() << "FilterStack::process(): Failed to process filter. Filter does not contain valid backend object.";
#endif
            return false;
        }

        const auto result = filter->process(
                                destination,
                                source
                            );
        assert( result );

        if( !result ) {
#ifdef LIBGRAPHICS_DEBUG_OUTPUT
            qDebug() << "FilterStack::process(): Failed to process filter. Aborting operation...";
#endif
            return false;
        }
    }

    return true;
}

bool FilterStack::process(
    fxapi::ApiBackendDevice*    device,
    libgraphics::ImageLayer*    destination,
    libgraphics::ImageLayer*    source
) {
    assert( device );
    assert( destination );
    assert( source );

    for( auto it = this->m_Filters.begin(); it != this->m_Filters.end(); ++it ) {
        auto filter = ( *it );

        const auto result = filter->process(
                                device,
                                destination,
                                source
                            );
        assert( result );

        if( !result ) {
#ifdef LIBGRAPHICS_DEBUG_OUTPUT
            qDebug() << "FilterStack::process(): Failed to process filter. Aborting operation...";
#endif
            return false;
        }
    }

    return true;
}

}
