//
// Retriever.h
//
// $Id: Retriever.h,v 1.7 1999/01/17 20:32:21 ghutchis Exp $
//
// $Log: Retriever.h,v $
// Revision 1.7  1999/01/17 20:32:21  ghutchis
// Added support for url_log, save and restart digs.
//
// Revision 1.6  1998/12/05 00:52:55  ghutchis
//
// Added a parameter to Initial function to prevent URLs from being checked
// twice during an update dig.
//
// Revision 1.5  1998/11/27 18:34:52  ghutchis
//
// Changed Retriever::got_word to check for small words, valid_punctuation to
// remove bugs in HTML.cc.
//
// Revision 1.4  1998/09/07 04:37:16  ghutchis
//
// Added DocState for documents marked as "noindex".
//
// Revision 1.3  1998/08/11 08:58:32  ghutchis
// Second patch for META description tags. New field in DocDB for the
// desc., space in word DB w/ proper factor.
//
// Revision 1.2  1998/07/09 09:39:00  ghutchis
//
//
// Added support for local file digging using patches by Pasi. Patches
// include support for local user (~username) digging.
//
// Revision 1.1.1.1  1997/02/03 17:11:06  turtle
// Initial CVS
//
// Revision 1.1  1995/12/11 22:47:02  turtle
// This uses the backwards model of only parsing HTML
//
// Revision 1.0  1995/08/18 16:27:04  turtle
// Before change to use of Server class
//
//
#ifndef _Retriever_h_
#define _Retriever_h_

#include "DocumentRef.h"
#include "Images.h"
#include <Dictionary.h>
#include <Queue.h>
#include <List.h>

class URL;
class Document;
class URLRef;

enum  RetrieverLog {
    Retriever_noLog,
    Retriever_logUrl,
    Retriever_Restart,
};

class Retriever
{
public:
    //
    // Construction/Destruction
    //
    			Retriever(RetrieverLog flags = Retriever_noLog);
    virtual		~Retriever();

    //
    // Getting it all started
    //
    void		Initial(char *url, int checked = 0);
    void		Initial(List &list , int checked = 0);
    void		Start();

    //
    // Report statistics about the parser
    //
    void		ReportStatistics(char *name);
	
    //
    // These are the callbacks that we need to write code for
    //
    void		got_word(char *word, int location, int heading);
    void		got_href(URL &url, char *description);
    void		got_title(char *title);
    void		got_head(char *head);
    void		got_meta_dsc(char *md);
    void		got_anchor(char *anchor);
    void		got_image(char *src);
    void		got_meta_email(char *);
    void		got_meta_notification(char *);
    void		got_meta_subject(char *);
    void                got_noindex();

    //
    // Allow for the indexing of protected sites by using a
    // username/password
    //
	void		setUsernamePassword(char *credentials);
	
private:
    //
    // A hash to keep track of what we've seen
    //
    Dictionary		visited;
    
    URL			*base;
    String		current_title;
    String		current_head;
    String		current_meta_dsc;
    int			current_id;
    DocumentRef		*current_ref;
    int			current_anchor_number;
    int			trackWords;
    int			n_links;
    Images		images;
    String		credentials;
	
    RetrieverLog log;
    //
    // These are weights for the words.  The index is the heading level.
    //
    double		factor[12];
    int			currenthopcount;

    //
    // Some semi-constants...
    //
    int			max_hop_count;
	
    //
    // The list of server-specific information objects is indexed by
    // ip address and port number.  The list contains Server objects.
    //
    Dictionary		servers;

    //
    // For efficiency reasons, we will only use one document object which
    // we reuse.
    //
    Document		*doc;

    String		notFound;

    // Some useful constants
    char             *valid_punctuation;
    int              minimumWordLength;

    //
    // Helper routines
    //
    int			Need2Get(char *url);
    DocumentRef	*	GetRef(char *url);
    int			IsValidURL(char *url);
    String *            IsLocal(char *url);
    String *            IsLocalUser(char *url);
    void		RetrievedDocument(Document &, char *url, DocumentRef *ref);
    void		parse_url(URLRef &urlRef);
    void		got_redirect(char *, DocumentRef *);
    void		recordNotFound(char *url, char *referer, int reason);
};

#endif


