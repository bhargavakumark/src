import java.io.IOException;
import java.net.UnknownHostException;
import java.util.List;

import netapp.manage.NaElement;
import netapp.manage.NaException;
import netapp.manage.NaServer;

public class ApiClient {

    public static void main(String[] args) {
        try {
            NaServer s = new NaServer("BK-NTAP8-VSRV1", 1 , 21);
            s.setServerType(NaServer.SERVER_TYPE_FILER);
            s.setTransportType(NaServer.TRANSPORT_TYPE_HTTPS);
            s.setPort(443);
            s.setStyle(NaServer.STYLE_LOGIN_PASSWORD);
            s.setAdminUser("vsadmin", "<password>");


            NaElement api = new NaElement("cifs-share-create");
            api.addNewChild("attribute-cache-ttl","<attribute-cache-ttl>");
            api.addNewChild("comment","<comment>");
            api.addNewChild("dir-umask","<dir-umask>");
            api.addNewChild("file-umask","<file-umask>");
            api.addNewChild("path","<path>");
            api.addNewChild("return-record","<return-record>");
            api.addNewChild("share-name","<share-name>");

            NaElement xi = new NaElement("share-properties");
            api.addChildElem(xi);

            xi.addNewChild("cifs-share-properties","<cifs-share-properties>");

            NaElement xi1 = new NaElement("symlink-properties");
            api.addChildElem(xi1);

            xi1.addNewChild("cifs-share-symlink-properties","<cifs-share-symlink-properties>");

            NaElement xo = s.invokeElem(api);
            System.out.println(xo.toPrettyString(""));


        } catch (NaException e) {
            handleException(e);
        } catch (UnknownHostException e) {
            handleException(e);
        } catch (IOException e) {
            handleException(e);
        }
    }

    private static void handleException(Exception e) {
        System.out.println(e.getMessage());
        e.printStackTrace();        
    }
}

