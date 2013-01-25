#include "cmusicfamitrackerinstrumentsmodel.h"

#include "stdio.h"

#include <QIcon>
#include <QFont>

static char modelStringBuffer [ 256 ];

CMusicFamiTrackerInstrumentsModel::CMusicFamiTrackerInstrumentsModel(CFamiTrackerDoc* pDoc,QObject*)
{
   m_pDocument = pDoc;
}

CMusicFamiTrackerInstrumentsModel::~CMusicFamiTrackerInstrumentsModel()
{
}

QVariant CMusicFamiTrackerInstrumentsModel::data(const QModelIndex& index, int role) const
{
   CInstrument* pInst = (CInstrument*)index.internalPointer();
   if (!index.isValid())
   {
      return QVariant();
   }

   if (role == Qt::FontRole)
   {
#ifdef Q_WS_MAC
      return QFont("Monaco",9);
#endif
#ifdef Q_WS_X11
      return QFont("Monospace",8);
#endif
#ifdef Q_WS_WIN
      return QFont("Consolas",9);
#endif
   }

   if ( pInst )
   {
      if (role == Qt::DecorationRole)
      {
         switch ( pInst->GetType() )
         {
         case INST_2A03:
            return QIcon(":/resources/Inst_2A03inv.ico");
            break;
         case INST_VRC6:
            return QIcon(":/resources/ico00001.ico");
            break;
         case INST_VRC7:
            return QIcon(":/resources/ico00003.ico");
            break;
         case INST_N163:
            return QIcon(":/resources/ico00004.ico");
            break;
         case INST_FDS:
            return QIcon(":/resources/icon2.ico");
            break;
         case INST_S5B:
            return QIcon(":/resources/ico00005.ico");
            break;
//         case INST_2A07:
//            return QIcon(":/resources/icon1.ico");
//            break;
         }
      }
      
      if (role != Qt::DisplayRole)
      {
         return QVariant();
      }
      
      sprintf(modelStringBuffer,"%02X - %s",index.row(),pInst->GetName());

      return QVariant(modelStringBuffer);
   }   

   return QVariant();
}

Qt::ItemFlags CMusicFamiTrackerInstrumentsModel::flags(const QModelIndex& /*index*/) const
{
   Qt::ItemFlags flags = Qt::ItemIsEnabled | Qt::ItemIsSelectable;

   return flags;
}

QModelIndex CMusicFamiTrackerInstrumentsModel::index(int row, int column, const QModelIndex&) const
{
   if ( (row >= 0) && (column >= 0) )
   {
      // Instrument slots might be empty...
      int idx;
      
      for ( idx = row; idx < MAX_INSTRUMENTS; idx++ )
      {
         if ( m_pDocument->IsInstrumentUsed(idx) )
         {
            return createIndex(row,column,m_pDocument->GetInstrument(idx));
         }
      }
   }
   
   return QModelIndex();
}

int CMusicFamiTrackerInstrumentsModel::rowCount(const QModelIndex&) const
{
   return MAX_INSTRUMENTS; //m_pDocument->GetInstrumentCount();
}

void CMusicFamiTrackerInstrumentsModel::update()
{
   emit layoutChanged();
}
